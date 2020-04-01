; ModuleID = '/home/augusto/Developer/llvm/switch-finder/test//test.c'
source_filename = "/home/augusto/Developer/llvm/switch-finder/test//test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.ST = type { i32, double }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @f(i32* %0, i32 %1) #0 {
  %3 = alloca i32*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32* %0, i32** %3, align 8
  store i32 %1, i32* %4, align 4
  store i32 0, i32* %5, align 4
  store i32 0, i32* %6, align 4
  br label %7

7:                                                ; preds = %32, %2
  %8 = load i32, i32* %6, align 4
  %9 = load i32, i32* %4, align 4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %35

11:                                               ; preds = %7
  %12 = load i32*, i32** %3, align 8
  %13 = load i32, i32* %6, align 4
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds i32, i32* %12, i64 %14
  %16 = load i32, i32* %15, align 4
  %17 = srem i32 %16, 4
  switch i32 %17, label %31 [
    i32 1, label %18
    i32 2, label %23
    i32 3, label %28
  ]

18:                                               ; preds = %11
  %19 = load i32, i32* %6, align 4
  %20 = mul nsw i32 %19, 2
  %21 = load i32, i32* %5, align 4
  %22 = add nsw i32 %21, %20
  store i32 %22, i32* %5, align 4
  br label %23

23:                                               ; preds = %11, %18
  %24 = load i32, i32* %6, align 4
  %25 = mul nsw i32 %24, 3
  %26 = load i32, i32* %5, align 4
  %27 = add nsw i32 %26, %25
  store i32 %27, i32* %5, align 4
  br label %28

28:                                               ; preds = %11, %23
  %29 = load i32, i32* %5, align 4
  %30 = add nsw i32 %29, 54
  store i32 %30, i32* %5, align 4
  br label %31

31:                                               ; preds = %28, %11
  br label %32

32:                                               ; preds = %31
  %33 = load i32, i32* %6, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, i32* %6, align 4
  br label %7

35:                                               ; preds = %7
  %36 = load i32, i32* %5, align 4
  ret i32 %36
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @g(i32 %0, double %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca %struct.ST, align 8
  %5 = bitcast %struct.ST* %4 to { i32, double }*
  %6 = getelementptr inbounds { i32, double }, { i32, double }* %5, i32 0, i32 0
  store i32 %0, i32* %6, align 8
  %7 = getelementptr inbounds { i32, double }, { i32, double }* %5, i32 0, i32 1
  store double %1, double* %7, align 8
  %8 = getelementptr inbounds %struct.ST, %struct.ST* %4, i32 0, i32 0
  %9 = load i32, i32* %8, align 8
  %10 = srem i32 %9, 4
  switch i32 %10, label %20 [
    i32 1, label %11
    i32 2, label %15
    i32 3, label %19
  ]

11:                                               ; preds = %2
  %12 = getelementptr inbounds %struct.ST, %struct.ST* %4, i32 0, i32 0
  %13 = load i32, i32* %12, align 8
  %14 = mul nsw i32 %13, 2
  store i32 %14, i32* %3, align 4
  br label %21

15:                                               ; preds = %2
  %16 = getelementptr inbounds %struct.ST, %struct.ST* %4, i32 0, i32 0
  %17 = load i32, i32* %16, align 8
  %18 = mul nsw i32 %17, 3
  store i32 %18, i32* %3, align 4
  br label %21

19:                                               ; preds = %2
  store i32 54, i32* %3, align 4
  br label %21

20:                                               ; preds = %2
  store i32 0, i32* %3, align 4
  br label %21

21:                                               ; preds = %20, %19, %15, %11
  %22 = load i32, i32* %3, align 4
  ret i32 %22
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @h(%struct.ST** %0, i32 %1) #0 {
  %3 = alloca %struct.ST**, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store %struct.ST** %0, %struct.ST*** %3, align 8
  store i32 %1, i32* %4, align 4
  store i32 0, i32* %5, align 4
  store i32 0, i32* %6, align 4
  br label %7

7:                                                ; preds = %34, %2
  %8 = load i32, i32* %6, align 4
  %9 = load i32, i32* %4, align 4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %37

11:                                               ; preds = %7
  %12 = load %struct.ST**, %struct.ST*** %3, align 8
  %13 = load i32, i32* %6, align 4
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds %struct.ST*, %struct.ST** %12, i64 %14
  %16 = load %struct.ST*, %struct.ST** %15, align 8
  %17 = getelementptr inbounds %struct.ST, %struct.ST* %16, i32 0, i32 0
  %18 = load i32, i32* %17, align 8
  %19 = srem i32 %18, 4
  switch i32 %19, label %33 [
    i32 1, label %20
    i32 2, label %25
    i32 3, label %30
  ]

20:                                               ; preds = %11
  %21 = load i32, i32* %6, align 4
  %22 = mul nsw i32 %21, 2
  %23 = load i32, i32* %5, align 4
  %24 = add nsw i32 %23, %22
  store i32 %24, i32* %5, align 4
  br label %25

25:                                               ; preds = %11, %20
  %26 = load i32, i32* %6, align 4
  %27 = mul nsw i32 %26, 3
  %28 = load i32, i32* %5, align 4
  %29 = add nsw i32 %28, %27
  store i32 %29, i32* %5, align 4
  br label %30

30:                                               ; preds = %11, %25
  %31 = load i32, i32* %5, align 4
  %32 = add nsw i32 %31, 54
  store i32 %32, i32* %5, align 4
  br label %33

33:                                               ; preds = %30, %11
  br label %34

34:                                               ; preds = %33
  %35 = load i32, i32* %6, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, i32* %6, align 4
  br label %7

37:                                               ; preds = %7
  %38 = load i32, i32* %5, align 4
  ret i32 %38
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-++20200323042644+d32170dbd5b-1~exp1~20200323154014.129 "}
