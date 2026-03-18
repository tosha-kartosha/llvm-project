// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCast.so -x c++ -S %s -o /dev/null 2>&1 | FileCheck %s
int main() {
    const int x = 5;
    int y = x; // здесь не просто lval, но с const int -> int - это implicit
}

//  CHECK: Function `main`:
//  CHECK-NEXT: Kind: LValueToRValue: const int -> int
//  CHECK-NEXT: -------------------------
//  CHECK-NEXT: Total implicit casts: 1