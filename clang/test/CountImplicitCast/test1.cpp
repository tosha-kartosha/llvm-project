// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCast.so -x c++ -S %s -o /dev/null 2>&1 | FileCheck %s
double sum(int a, float b) {
    return a + b;
}

int mul(float a, float b) {
	return a + sum(a, b);
}

// CHECK: Function `sum`:
// CHECK-NEXT: Kind: FloatingCast: float -> double
// CHECK-NEXT: Kind: IntegralToFloating: int -> float
// CHECK-EMPTY:
// CHECK-NEXT: Function `mul`:
// CHECK-NEXT: Kind: FloatingToIntegral: double -> int
// CHECK-NEXT: Kind: FloatingCast: float -> double
// CHECK-NEXT: Kind: FunctionToPointerDecay: double (int, float) -> double (*)(int, float)
// CHECK-NEXT: Kind: FloatingToIntegral: float -> int
// CHECK-NEXT: -------------------------
// CHECK-NEXT: Total implicit casts: 6

// CHECK-NOT: Kind: LValueToRValue: float -> float
// CHECK-NOT: Kind: LValueToRValue: int -> int