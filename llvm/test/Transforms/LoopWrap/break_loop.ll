; RUN: opt -passes=loop-wrap -S %s | FileCheck %s

define dso_local noundef i32 @test_with_break(i32 noundef %n) #0 {
entry:
  %n.addr = alloca i32, align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %sum, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:
  %2 = load i32, ptr %i, align 4
  %cmp1 = icmp eq i32 %2, 5
  br i1 %cmp1, label %if.then, label %if.end

if.then:
  br label %for.end

if.end:
  %3 = load i32, ptr %i, align 4
  %4 = load i32, ptr %sum, align 4
  %add = add nsw i32 %4, %3
  store i32 %add, ptr %sum, align 4
  br label %for.inc

for.inc:
  %5 = load i32, ptr %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond

for.end:
  %6 = load i32, ptr %sum, align 4
  ret i32 %6
}
; CHECK: entry:
; CHECK: call void @loop_start()
; CHECK: if.then:
; CHECK: call void @loop_end()
; CHECK: for.end:
; CHECK: call void @loop_end()