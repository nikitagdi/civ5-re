; ModuleID = 'src/p.cpp'
source_filename = "src/p.cpp"
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-i128:128-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

$_ZN11__typedb_P1D2Ev = comdat any

$_ZN11__typedb_P1D0Ev = comdat any

$_ZN1DD2Ev = comdat any

@_ZN3Gfx11__typedb_f1E = dso_local global ptr @_ZN3Gfx4LoadEfPNS_3TexE, align 4, !dbg !0
@__typedb_s1 = dso_local global [3 x i32] [i32 12, i32 4, i32 12], align 4, !dbg !12
@pf = dso_local global ptr @"odd$name", align 4, !dbg !17
@_ZTV11__typedb_P1 = dso_local unnamed_addr constant { [7 x ptr], [3 x ptr] } { [7 x ptr] [ptr null, ptr @_ZTI11__typedb_P1, ptr @_ZN11__typedb_P1D2Ev, ptr @_ZN11__typedb_P1D0Ev, ptr @_ZN1B1gEv, ptr @_ZN1D7OnEventEi, ptr @_ZN11__typedb_P110__typedb_kEv], [3 x ptr] [ptr inttoptr (i32 -8 to ptr), ptr @_ZTI11__typedb_P1, ptr @_ZThn8_N1D7OnEventEi] }, align 4, !dbg !22
@_ZTI11__typedb_P1 = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTVN10__cxxabiv120__si_class_type_infoE, i32 2), ptr @_ZTS11__typedb_P1, ptr @_ZTI1D }, align 4
@_ZTVN10__cxxabiv120__si_class_type_infoE = external global [0 x ptr]
@_ZTS11__typedb_P1 = dso_local constant [14 x i8] c"11__typedb_P1\00", align 1
@_ZTI1D = external constant ptr

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_ZN11__typedb_P110__typedb_kEv(ptr noundef nonnull align 4 dereferenceable(12) %0) unnamed_addr #0 align 2 !dbg !75 {
  %2 = alloca ptr, align 4
  store ptr %0, ptr %2, align 4
    #dbg_declare(ptr %2, !76, !DIExpression(), !78)
  %3 = load ptr, ptr %2, align 4
  ret void, !dbg !79
}

declare void @_ZN3Gfx4LoadEfPNS_3TexE(float noundef, ptr noundef) #1

declare void @"odd$name"() #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN11__typedb_P1D2Ev(ptr noundef nonnull align 4 dereferenceable(12) %0) unnamed_addr #0 comdat align 2 !dbg !80 {
  %2 = alloca ptr, align 4
  store ptr %0, ptr %2, align 4
    #dbg_declare(ptr %2, !82, !DIExpression(), !83)
  %3 = load ptr, ptr %2, align 4
  call void @_ZN1DD2Ev(ptr noundef nonnull align 4 dereferenceable(12) %3) #4, !dbg !84
  ret void, !dbg !86
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN11__typedb_P1D0Ev(ptr noundef nonnull align 4 dereferenceable(12) %0) unnamed_addr #0 comdat align 2 !dbg !87 {
  %2 = alloca ptr, align 4
  store ptr %0, ptr %2, align 4
    #dbg_declare(ptr %2, !88, !DIExpression(), !89)
  %3 = load ptr, ptr %2, align 4
  call void @_ZN11__typedb_P1D2Ev(ptr noundef nonnull align 4 dereferenceable(12) %3) #4, !dbg !90
  call void @_ZdlPv(ptr noundef %3) #5, !dbg !90
  ret void, !dbg !90
}

declare void @_ZN1B1gEv(ptr noundef nonnull align 4 dereferenceable(8)) unnamed_addr #1

declare void @_ZN1D7OnEventEi(ptr noundef nonnull align 4 dereferenceable(12), i32 noundef) unnamed_addr #1

declare void @_ZThn8_N1D7OnEventEi(ptr noundef, i32 noundef) unnamed_addr #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN1DD2Ev(ptr noundef nonnull align 4 dereferenceable(12) %0) unnamed_addr #0 comdat align 2 !dbg !91 {
  %2 = alloca ptr, align 4
  store ptr %0, ptr %2, align 4
    #dbg_declare(ptr %2, !95, !DIExpression(), !97)
  %3 = load ptr, ptr %2, align 4
  call void @_ZN1BD2Ev(ptr noundef nonnull align 4 dereferenceable(8) %3) #4, !dbg !98
  ret void, !dbg !100
}

; Function Attrs: nounwind
declare void @_ZN1BD2Ev(ptr noundef nonnull align 4 dereferenceable(8)) unnamed_addr #2

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(ptr noundef) #3

attributes #0 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nobuiltin nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }
attributes #5 = { builtin nounwind }

!llvm.dbg.cu = !{!14}
!llvm.module.flags = !{!66, !67, !68, !69, !70, !71, !72, !73}


!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "__typedb_f1", linkageName: "_ZN3Gfx11__typedb_f1E", scope: !2, file: !3, line: 8, type: !4, isLocal: false, isDefinition: true)
!2 = !DINamespace(name: "Gfx", scope: null)
!3 = !DIFile(filename: "src/p.cpp", directory: "", checksumkind: CSK_MD5, checksum: "a9da5a599b99c0b0f6e23bc5929331c3")
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 32)
!5 = !DIDerivedType(tag: DW_TAG_typedef, name: "__typedb_id<void (float, Gfx::Tex *)>", file: !3, line: 3, baseType: !6)
!6 = !DISubroutineType(types: !7)
!7 = !{null, !8, !9}
!8 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 32)
!10 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "Tex", scope: !2, file: !3, line: 2, size: 8, flags: DIFlagTypePassByValue, elements: !11, identifier: "_ZTSN3Gfx3TexE")
!11 = !{}
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(name: "__typedb_s1", scope: !14, file: !3, line: 9, type: !62, isLocal: false, isDefinition: true)
!14 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_11, file: !15, producer: "clang version 22.1.0 (https://github.com/llvm/llvm-project 4434dabb69916856b824f68a64b029c67175e532)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !16, splitDebugInlining: false, nameTableKind: None)
!15 = !DIFile(filename: "src\\p.cpp", directory: "", checksumkind: CSK_MD5, checksum: "a9da5a599b99c0b0f6e23bc5929331c3")
!16 = !{!0, !12, !17, !22}
!17 = !DIGlobalVariableExpression(var: !18, expr: !DIExpression())
!18 = distinct !DIGlobalVariable(name: "pf", scope: !14, file: !3, line: 11, type: !19, isLocal: false, isDefinition: true)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 32)
!20 = !DISubroutineType(types: !21)
!21 = !{null}
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "_vtable$", linkageName: "_ZTV11__typedb_P1", scope: !14, file: !15, type: !24, isLocal: false, isDefinition: true, declaration: !25, align: 32)
!24 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!25 = !DIDerivedType(tag: DW_TAG_variable, name: "_vtable$", scope: !26, file: !15, baseType: !24, flags: DIFlagPrivate | DIFlagArtificial | DIFlagStaticMember)
!26 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__typedb_P1", file: !3, line: 7, size: 96, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !27, vtableHolder: !32)
!27 = !{!28, !58}
!28 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !26, baseType: !29, extraData: i32 0)
!29 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "D", file: !3, line: 6, size: 96, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !30, vtableHolder: !32)
!30 = !{!31, !46, !54}
!31 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !29, baseType: !32, extraData: i32 0)
!32 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "B", file: !3, line: 4, size: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !33, vtableHolder: !32)
!33 = !{!34, !40, !41, !45}
!34 = !DIDerivedType(tag: DW_TAG_member, name: "_vptr$B", scope: !3, file: !3, baseType: !35, size: 32, flags: DIFlagArtificial)
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !36, size: 32)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, name: "__vtbl_ptr_type", baseType: !37, size: 32)
!37 = !DISubroutineType(types: !38)
!38 = !{!39}
!39 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "m", scope: !32, file: !3, line: 4, baseType: !39, size: 32, offset: 32)
!41 = !DISubprogram(name: "~B", linkageName: "_ZN1BD4Ev", scope: !32, file: !3, line: 4, type: !42, scopeLine: 4, containingType: !32, virtualIndex: 0, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!42 = !DISubroutineType(types: !43)
!43 = !{null, !44}
!44 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !32, size: 32, flags: DIFlagArtificial | DIFlagObjectPointer)
!45 = !DISubprogram(name: "g", linkageName: "_ZN1B1gEv", scope: !32, file: !3, line: 4, type: !42, scopeLine: 4, containingType: !32, virtualIndex: 2, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!46 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !29, baseType: !47, offset: 64, extraData: i32 0)
!47 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "E", file: !3, line: 5, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !48, vtableHolder: !47, identifier: "_ZTS1E")
!48 = !{!49, !50}
!49 = !DIDerivedType(tag: DW_TAG_member, name: "_vptr$E", scope: !3, file: !3, baseType: !35, size: 32, flags: DIFlagArtificial)
!50 = !DISubprogram(name: "OnEvent", linkageName: "_ZN1E7OnEventEi", scope: !47, file: !3, line: 5, type: !51, scopeLine: 5, containingType: !47, virtualIndex: 0, flags: DIFlagPrototyped, spFlags: DISPFlagPureVirtual)
!51 = !DISubroutineType(types: !52)
!52 = !{null, !53, !39}
!53 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !47, size: 32, flags: DIFlagArtificial | DIFlagObjectPointer)
!54 = !DISubprogram(name: "OnEvent", linkageName: "_ZN1D7OnEventEi", scope: !29, file: !3, line: 6, type: !55, scopeLine: 6, containingType: !29, virtualIndex: 3, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!55 = !DISubroutineType(types: !56)
!56 = !{null, !57, !39}
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 32, flags: DIFlagArtificial | DIFlagObjectPointer)
!58 = !DISubprogram(name: "__typedb_k", linkageName: "_ZN11__typedb_P110__typedb_kEv", scope: !26, file: !3, line: 7, type: !59, scopeLine: 7, containingType: !26, virtualIndex: 4, flags: DIFlagPrototyped, spFlags: DISPFlagVirtual)
!59 = !DISubroutineType(types: !60)
!60 = !{null, !61}
!61 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !26, size: 32, flags: DIFlagArtificial | DIFlagObjectPointer)
!62 = !DICompositeType(tag: DW_TAG_array_type, baseType: !63, size: 96, elements: !64)
!63 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!64 = !{!65}
!65 = !DISubrange(count: 3)
!66 = !{i32 1, !"NumRegisterParameters", i32 0}
!67 = !{i32 7, !"Dwarf Version", i32 5}
!68 = !{i32 2, !"Debug Info Version", i32 3}
!69 = !{i32 1, !"wchar_size", i32 4}
!70 = !{i32 8, !"PIC Level", i32 2}
!71 = !{i32 7, !"PIE Level", i32 2}
!72 = !{i32 7, !"uwtable", i32 2}
!73 = !{i32 7, !"frame-pointer", i32 2}

!75 = distinct !DISubprogram(name: "__typedb_k", linkageName: "_ZN11__typedb_P110__typedb_kEv", scope: !26, file: !3, line: 7, type: !59, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !14, declaration: !58, retainedNodes: !11)
!76 = !DILocalVariable(name: "this", arg: 1, scope: !75, type: !77, flags: DIFlagArtificial | DIFlagObjectPointer)
!77 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !26, size: 32)
!78 = !DILocation(line: 0, scope: !75)
!79 = !DILocation(line: 7, column: 88, scope: !75)
!80 = distinct !DISubprogram(name: "~__typedb_P1", linkageName: "_ZN11__typedb_P1D2Ev", scope: !26, file: !3, line: 7, type: !59, scopeLine: 7, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !14, declaration: !81, retainedNodes: !11)
!81 = !DISubprogram(name: "~__typedb_P1", linkageName: "_ZN11__typedb_P1D4Ev", scope: !26, type: !59, containingType: !26, virtualIndex: 0, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagVirtual)
!82 = !DILocalVariable(name: "this", arg: 1, scope: !80, type: !77, flags: DIFlagArtificial | DIFlagObjectPointer)
!83 = !DILocation(line: 0, scope: !80)
!84 = !DILocation(line: 7, column: 8, scope: !85)
!85 = distinct !DILexicalBlock(scope: !80, file: !3, line: 7, column: 8)
!86 = !DILocation(line: 7, column: 8, scope: !80)
!87 = distinct !DISubprogram(name: "~__typedb_P1", linkageName: "_ZN11__typedb_P1D0Ev", scope: !26, file: !3, line: 7, type: !59, scopeLine: 7, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !14, declaration: !81, retainedNodes: !11)
!88 = !DILocalVariable(name: "this", arg: 1, scope: !87, type: !77, flags: DIFlagArtificial | DIFlagObjectPointer)
!89 = !DILocation(line: 0, scope: !87)
!90 = !DILocation(line: 7, column: 8, scope: !87)
!91 = distinct !DISubprogram(name: "~D", linkageName: "_ZN1DD2Ev", scope: !29, file: !3, line: 6, type: !92, scopeLine: 6, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !14, declaration: !94, retainedNodes: !11)
!92 = !DISubroutineType(types: !93)
!93 = !{null, !57}
!94 = !DISubprogram(name: "~D", linkageName: "_ZN1DD4Ev", scope: !29, type: !92, containingType: !29, virtualIndex: 0, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagVirtual)
!95 = !DILocalVariable(name: "this", arg: 1, scope: !91, type: !96, flags: DIFlagArtificial | DIFlagObjectPointer)
!96 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 32)
!97 = !DILocation(line: 0, scope: !91)
!98 = !DILocation(line: 6, column: 8, scope: !99)
!99 = distinct !DILexicalBlock(scope: !91, file: !3, line: 6, column: 8)
!100 = !DILocation(line: 6, column: 8, scope: !91)
