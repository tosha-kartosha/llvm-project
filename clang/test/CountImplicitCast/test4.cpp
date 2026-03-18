// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCast.so -x c++ -S %s -o /dev/null 2>&1 | FileCheck %s
static float st_flow = 42;
float f = 42;

int main() {
    int m = f + 10;
    return 0;
}

// CHECK: Global var:
// CHECK-NEXT: Kind: IntegralToFloating: int -> float
// CHECK-EMPTY:
// CHECK-NEXT: Global var:
// CHECK-NEXT: Kind: IntegralToFloating: int -> float
// CHECK-NEXT: Function `main`:
// CHECK-EMPTY:
// CHECK-NEXT: Kind: FloatingToIntegral: float -> int
// CHECK-NEXT: Kind: IntegralToFloating: int -> float
// CHECK-NEXT: -------------------------
// CHECK-NEXT: Total implicit casts: 4

// CHECK-NOT: Kind: LValueToRValue: float -> float