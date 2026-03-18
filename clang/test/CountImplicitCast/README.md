# Краткая информация по тестам и запуску
- есть 4 теста: 1 шаблон и 3 выдумки
    1. шаблон из godbolt
    2. на то, что explicit не обрабатывает + пример с указателями в конце
    3. на const int -> int
    4. на глобальные переменные

- шаблонный вывод: `Kind: <CastKind>: SrcType -> DstType`
- программа допускает неявные преобразования вида FunctionToPointerDecay и LValueToRValue в том случае, когда у них меняется тип src и dst
- cmake делала через: `cmake --build . --target CountImplicitCast`
- lit-тесты проверяла через: `bin/llvm-lit tools/clang/test/CountImplicitCast`
