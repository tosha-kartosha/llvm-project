; RUN: opt -passes=loop-wrap -S %s | FileCheck %s

define void @test_nested(i32 %n, i32 %m) {
entry:
  %n.addr = alloca i32
  %m.addr = alloca i32
  %i = alloca i32
  %j = alloca i32
  store i32 %n, ptr %n.addr
  store i32 %m, ptr %m.addr
  store i32 0, ptr %i
  br label %outer.cond

outer.cond:
  %0 = load i32, ptr %i
  %1 = load i32, ptr %n.addr
  %cmp_outer = icmp slt i32 %0, %1
  br i1 %cmp_outer, label %outer.body, label %outer.end

outer.body:
  store i32 0, ptr %j
  br label %inner.cond

inner.cond:
  %2 = load i32, ptr %j
  %3 = load i32, ptr %m.addr
  %cmp_inner = icmp slt i32 %2, %3
  br i1 %cmp_inner, label %inner.body, label %inner.end

inner.body:
  %4 = load i32, ptr %j
  %inc_inner = add nsw i32 %4, 1
  store i32 %inc_inner, ptr %j
  br label %inner.cond

inner.end:
  %5 = load i32, ptr %i
  %inc_outer = add nsw i32 %5, 1
  store i32 %inc_outer, ptr %i
  br label %outer.cond

outer.end:
  ret void
}

; CHECK: entry:
; CHECK: call void @loop_start()
; CHECK: outer.body:
; CHECK: call void @loop_start()
; CHECK: inner.end:
; CHECK: call void @loop_end()
; CHECK: outer.end:
; CHECK: call void @loop_end()

