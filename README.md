<div align="center">
    <h1>graphite</h1>
    <h2>Fast (🚀), Memory safe (🚀), Simple (🚀) language compiler written in C++ (bad 😡) using LLVM (🚀)</h2>
</div>

<div align="center">
  <img alt="Stargazers" src="https://img.shields.io/github/stars/budchirp/graphite?style=for-the-badge&colorA=0b1221&colorB=ff8e8e" />
  <img alt="Last commit" src="https://img.shields.io/github/last-commit/budchirp/graphite?style=for-the-badge&colorA=0b1221&colorB=BDB0E4" />
  <img alt="Issues" src="https://img.shields.io/github/issues/budchirp/graphite?style=for-the-badge&colorA=0b1221&colorB=FBC19D" />
</div>

## TODO

- [x] Lexer
- [x] Parser
- [x] Codegen
- [x] Type support & checking
- [x] Function definition
- [x] Variable definition
- [x] Mutable/Immutable variables (immutable by default btw)
- [x] Null (?, !!, null)
- [x] Binary expressions (=, ==, !=, +, -, /, *)
- [x] Unary expressions (*, &, -, +, ++, --, !)
- [x] If expression
- [x] For & While statement
- [x] Arrays
- [x] Structs (just works nothing else)
- [x] Module management (public, private, include) (garbage btw)
- [x] Object generation
- [x] Code optimization (LLVM IR Optimization, no optimization from graphite)

- [ ] Stdlib
- [ ] Maps
- [ ] Scope management and ownership system similar to crab 🦀 (aka crap) or C++ RAII
- [ ] Switch expression
- [ ] CLI (graphite, graphitec)
- [ ] OOP

- [ ] Fix sturct gep issues
- [ ] Struct semantic analysis
- [ ] Make bang operator work with expressions (currently only supports variable reference)
- [ ] Improve my garbage code
- [ ] Fix stupid parser errors
