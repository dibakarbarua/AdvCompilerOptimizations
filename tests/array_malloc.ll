; ModuleID = 'array_malloc.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %retval
  %call = call noalias i8* @malloc(i64 400) #2
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %a, align 8
  %1 = load i32*, i32** %a, align 8
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 10
  store i32 6, i32* %arrayidx, align 4
  store i32 6, i32* %i, align 4
  %2 = load i32, i32* %i, align 4
  %mul = mul nsw i32 2, %2
  %add = add nsw i32 %mul, 5
  %idxprom = sext i32 %add to i64
  %3 = load i32*, i32** %a, align 8
  %arrayidx1 = getelementptr inbounds i32, i32* %3, i64 %idxprom
  store i32 45, i32* %arrayidx1, align 4
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
