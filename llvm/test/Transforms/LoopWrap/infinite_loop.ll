; RUN: opt -passes=loop-wrap -S %s | FileCheck %s

define dso_local void @infinite_while() #0 {
entry:
  %counter = alloca i32, align 4
  store i32 0, ptr %counter, align 4
  br label %while.body

while.body:
  %0 = load i32, ptr %counter, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr %counter, align 4
  br label %while.body
}

; CHECK: entry:
; CHECK: call void @loop_start()
; CHECK-NOT: call void @loop_end()