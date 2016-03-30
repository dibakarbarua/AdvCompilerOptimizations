; ModuleID = 'array_init.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca [100 x i32], align 16
  %i = alloca i32, align 4
  store i32 0, i32* %retval
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* %a, i32 0, i64 10
  store i32 6, i32* %arrayidx, align 4
  store i32 6, i32* %i, align 4
  %0 = load i32, i32* %i, align 4
  %mul = mul nsw i32 2, %0
  %add = add nsw i32 %mul, 5
  %idxprom = sext i32 %add to i64
  %arrayidx1 = getelementptr inbounds [100 x i32], [100 x i32]* %a, i32 0, i64 %idxprom
  store i32 45, i32* %arrayidx1, align 4
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
