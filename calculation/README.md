# Four Arithmetic Operations Program

## Contents

This is an interpreter written by C language.  
This reads a equation of four arithmetic operations and calculate it.  
Tokenize and parse is fulfilled without Lex and YACC.  
OCaml file attached to this project make equation automatically.

## Validation

C language: gcc 4.9.2
OCaml: ocaml 4.02.1

## Usage

assuming bash

## Compilation

```
bash compile.sh
```
or
```
make
```

## Execution Sample

```
shell$ bash execute.sh
> (3 + 2) * 11 - 2 * 7
41
> (-3*4+ 5) + (11 * 13 / 6 - 9) * (111 % 8)
91
```

## Cleanup

```
bash clean.sh
```
or
```
make clean
```

## Create an Equation Automatically

```
ocaml tree.ml $(depth)
```
where $(depth) is maximum depth of syntax tree.

# 四則演算プログラム

## 内容

C言語で記述された四則演算を行う対話型プログラム  
四則演算で記述された式を読み込み計算する。  
Lex や YACC を使わずに字句解析および構文解析を実現している。  
付属の OCaml file は式を自動的に生成する。

## 動作確認

C language: gcc 4.9.2
OCaml: ocaml 4.02.1

## 使用方法

以下のコマンドは bash を使う想定で記述されている。

## コンパイル方法

```
bash compile.sh
```
または
```
make
```

## 実行例

```
shell$ bash execute.sh
> (3 + 2) * 11 - 2 * 7
41
> (-3*4+ 5) + (11 * 13 / 6 - 9) * (111 % 8)
91
```

## クリーンアップ

```
bash clean.sh
```
または
```
make clean
```

## 式の自動生成

```
ocaml tree.ml $(depth)
```
ただし $(depth) は構文木の深さの最大値である。
