// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCast.so -x c++ -S %s -o /dev/null 2>&1 | FileCheck %s

int super_convert(double num_with_dot) {
    return (int) num_with_dot; // explicit внутри функции - должен вывести пустую функцию
}

int another_super_convert(double &dec1) {
	return static_cast<int>(dec1); // explicit внутри функции - должен вывести пустую функцию
}

int main() {
    int number = 5;
    double decimal = number; // implicit

    double num_with_dot = 19.99;
    int rounded = super_convert(num_with_dot); // ожидаем implicit: учитываем: функция → указатель

    double dec1 = 9.7;
    int num1 = another_super_convert(dec1); // ожидаем implicit: учитываем: функция → указатель

    bool global_flag = 42; // implicit

    int (*ptr)(double) = super_convert;  // учитываем: функция → указатель

	int (*ptr2)(double) = ptr; // не учтем: это lval -> rval с int (*)(double)
}

// CHECK: Function `super_convert`:
// CHECK-EMPTY:
// CHECK-NEXT: Function `another_super_convert`:
// CHECK-EMPTY:
// CHECK-NEXT: Function `main`:
// CHECK-NEXT: Kind: IntegralToFloating: int -> double
// CHECK-NEXT: Kind: FunctionToPointerDecay: int (double) -> int (*)(double)
// CHECK-NEXT: Kind: FunctionToPointerDecay: int (double &) -> int (*)(double &)
// CHECK-NEXT: Kind: IntegralToBoolean: int -> _Bool
// CHECK-NEXT: Kind: FunctionToPointerDecay: int (double) -> int (*)(double)
// CHECK-NEXT: -------------------------
// CHECK-NEXT: Total implicit casts: 5

// CHECK-NOT: Kind: LValueToRValue: double -> double
// CHECK-NOT: Kind: LValueToRValue: int -> int
// CHECK-NOT: Kind: LValueToRValue: int (*)(double) -> int (*)(double)