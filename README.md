# MiniEDA

A small EDA core toolchain written in modern C++17, built as a learning project: read a netlist, build a safe in-memory data model, and (in later stages) analyze connectivity, route nets, and optimize placement.

The goal is not to compete with commercial tools, but to implement each core step of an EDA flow by hand and understand why it is designed the way it is.

```
Netlist text -> Data model -> Analysis -> Routing -> Placement -> Output / visualization
   (done)         (done)      (planned)   (planned)   (planned)       (planned)
```

## Status

- [x] **Stage 1**: netlist data model (Component / Pin / Net / Design) and text parser with line-numbered errors
- [ ] **Stage 2**: connectivity analysis (fan-out, floating pins, HPWL wirelength estimate)
- [ ] **Stage 3**: grid maze routing (Lee / A*)
- [ ] **Stage 4**: placement optimization (simulated annealing)
- [ ] **Stage 5**: benchmarks, SVG output, documentation

## Netlist format

A netlist is a plain text file, one statement per line.

```
# Components: COMP <name> <pin-number>=<pin-name> ...
COMP U1 1=IN 2=GND 3=OUT
COMP R1 1=A 2=B
COMP C1 1=A 2=B

# Nets: NET <name> <component>.<pin-number> ...
NET VCC U1.1 R1.1
NET GND U1.2 C1.2
NET N1  R1.2 C1.1 U1.3
```

Rules:

- Empty lines and lines starting with `#` are ignored.
- Tokens are separated by any amount of spaces or tabs. Windows (`\r\n`) line endings are accepted.
- A component name must be unique in the design. A pin number must be unique within its component. A net name must be unique in the design.
- A pin has a **number** (its identity, e.g. `8`, `A1`) and a **name** (its function, e.g. `VCC`). Numbers are strings, so BGA-style pin numbers work. Names are not required to be unique, since one IC may have several `GND` pins.
- A `NET` may only reference components declared **earlier** in the file.
- In a pin reference such as `U1.8`, the split happens at the **last** `.`, so component names may contain dots.

### Error reporting

Every error is reported with its line number:

```
line 3: component not found: U9
line 5: unknown keyword: FOO
line 2: duplicate net name: GND
```

## Usage

```cpp
#include <sstream>
#include "parser.h"
#include "design.h"

std::istringstream in(
    "COMP U1 1=IN 2=GND\n"
    "COMP R1 1=A 2=B\n"
    "NET GND U1.2 R1.2\n");

Design design;
parseNetlist(in, design);          // throws std::runtime_error with a line number on bad input

Net& gnd = design.findNet("GND");
for (const Pin& pin : gnd.pins()) {
    // ...
}
```

`parseNetlist` takes a `std::istream&`, so it works with files (`std::ifstream`) and with in-memory strings (used heavily in the tests).

## Build and test

Requirements: a C++17 compiler (GCC or Clang), CMake 3.20 or newer. GoogleTest is downloaded automatically by CMake.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The test target is built with AddressSanitizer and UndefinedBehaviorSanitizer enabled, so memory errors (use-after-free, out-of-bounds) and undefined behavior fail the tests instead of passing silently.

## Design notes

The central question of the data model is **who owns what**. C++ has no garbage collector, so ownership has to be explicit.

- **`Component` owns its `Pin`s** through `std::vector<std::unique_ptr<Pin>>`. Each pin lives on the heap, so its address stays stable when the vector grows. Anything holding a reference to a pin is not invalidated by later `addPin` calls.
- **`Net` does not own pins.** It stores `std::reference_wrapper<const Pin>`, which expresses "I refer to this pin, I do not manage its life". Nets also only get read access to pins.
- **A `Pin` cannot exist on its own.** Its constructor is private (only `Component` is a friend) and copying is deleted. There is no way to build a stray pin and hand it to a net, so a whole class of dangling-reference bugs cannot be written.
- **`Design` owns everything** (components and nets), which gives the whole design one lifetime. Member declaration order matters: C++ destroys members in reverse order of declaration, so nets are declared after components and are destroyed first. A net is therefore never alive without the pins it refers to. The order is documented in the source because the compiler cannot check it.
- **Name lookup** uses `std::unordered_map<std::string, Component*>` (and the same for nets). These maps hold non-owning raw pointers, are declared last so they are destroyed first, and are filled from the owning object's stored name to avoid use-after-move mistakes.
- **Objects are valid on construction.** Names and numbers are constructor arguments, not setters, so there is no half-initialized Component or Pin to defend against.

### Known limitations

- A line that fails halfway leaves the objects it already created in the `Design`. Treat a `Design` as discarded when `parseNetlist` throws.
- The parser currently supports only this project's own text format. Importing real OrCAD / Allegro netlist exports is a possible later extension.
- References held by a `Net` are valid only as long as the owning `Design` is alive.

## Roadmap

1. **Connectivity analysis**: fan-out per net, floating pins, single-pin nets, and a half-perimeter wirelength (HPWL) estimate once components have coordinates.
2. **Maze routing**: Lee / A* on a grid with obstacles, plus a design-rule-style checker that verifies routed results (connected, no overlaps, no obstacle crossings).
3. **Placement**: simulated annealing to reduce total HPWL, with benchmark curves.
4. **Visualization**: SVG output of placement and routing results.