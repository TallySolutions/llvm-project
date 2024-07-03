//===--- TokenAnnotator.h - Format C++ code ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements a token annotator, i.e. creates
/// \c AnnotatedTokens out of \c FormatTokens with required extra information.
///
//===----------------------------------------------------------------------===//
// clang-format off
#ifndef LLVM_CLANG_LIB_FORMAT_TOKENANNOTATOR_H
#define LLVM_CLANG_LIB_FORMAT_TOKENANNOTATOR_H

#include "UnwrappedLineParser.h"
#include "clang/Format/Format.h"
#include <string>
#include <set>

using namespace std;

namespace clang {
namespace format {

enum LineType {
  LT_Invalid,
  LT_ImportStatement,
  LT_ObjCDecl, // An @interface, @implementation, or @protocol line.
  LT_ObjCMethodDecl,
  LT_ObjCProperty, // An @property line.
  LT_Other,
  LT_PreprocessorDirective,
  LT_VirtualFunctionDecl,
  LT_ArrayOfStructInitializer,
  LT_CommentAbovePPDirective,
};

enum ScopeType {
  // Contained in class declaration/definition.
  ST_Class,
  ST_Struct,
  ST_Union,
  // Contained within function definition.
  ST_Function,
  // Contained within other scope block (loop, if/else, etc).
  ST_Other,
};

class AnnotatedLine {
public:
  AnnotatedLine(const UnwrappedLine &Line)
      : First(Line.Tokens.front().Tok), Level(Line.Level),
        PPLevel(Line.PPLevel),
        MatchingOpeningBlockLineIndex(Line.MatchingOpeningBlockLineIndex),
        MatchingClosingBlockLineIndex(Line.MatchingClosingBlockLineIndex),
        InPPDirective(Line.InPPDirective),
        InPragmaDirective(Line.InPragmaDirective),
        InMacroBody(Line.InMacroBody),
        MustBeDeclaration(Line.MustBeDeclaration), MightBeFunctionDecl(false),
        IsMultiVariableDeclStmt(false), Affected(false),
        LeadingEmptyLinesAffected(false), ChildrenAffected(false),
        ReturnTypeWrapped(false), IsContinuation(Line.IsContinuation),
        FirstStartColumn(Line.FirstStartColumn), /* TALLY */ IsDoubleIndented(false) {
    assert(!Line.Tokens.empty());

    // Calculate Next and Previous for all tokens. Note that we must overwrite
    // Next and Previous for every token, as previous formatting runs might have
    // left them in a different state.
    First->Previous = nullptr;
    FormatToken *Current = First;
    addChildren(Line.Tokens.front(), Current);
    for (const UnwrappedLineNode &Node : llvm::drop_begin(Line.Tokens)) {
      if (Node.Tok->MacroParent)
        ContainsMacroCall = true;
      Current->Next = Node.Tok;
      Node.Tok->Previous = Current;
      Current = Current->Next;
      addChildren(Node, Current);
      // FIXME: if we add children, previous will point to the token before
      // the children; changing this requires significant changes across
      // clang-format.
    }
    Last = Current;
    Last->Next = nullptr;
  }

  void addChildren(const UnwrappedLineNode &Node, FormatToken *Current) {
    Current->Children.clear();
    for (const auto &Child : Node.Children) {
      Children.push_back(new AnnotatedLine(Child));
      if (Children.back()->ContainsMacroCall)
        ContainsMacroCall = true;
      Current->Children.push_back(Children.back());
    }
  }

  size_t size() const {
    size_t Size = 1;
    for (const auto *Child : Children)
      Size += Child->size();
    return Size;
  }

  ~AnnotatedLine() {
    for (AnnotatedLine *Child : Children)
      delete Child;
    FormatToken *Current = First;
    while (Current) {
      Current->Children.clear();
      Current->Role.reset();
      Current = Current->Next;
    }
  }

  bool isComment() const {
    return First && First->is(tok::comment) && !First->getNextNonComment();
  }

  /// \c true if this line starts with the given tokens in order, ignoring
  /// comments.
  template <typename... Ts> bool startsWith(Ts... Tokens) const {
    return First && First->startsSequence(Tokens...);
  }

  /// \c true if this line ends with the given tokens in reversed order,
  /// ignoring comments.
  /// For example, given tokens [T1, T2, T3, ...], the function returns true if
  /// this line is like "... T3 T2 T1".
  template <typename... Ts> bool endsWith(Ts... Tokens) const {
    return Last && Last->endsSequence(Tokens...);
  }

  /// \c true if this line looks like a function definition instead of a
  /// function declaration. Asserts MightBeFunctionDecl.
  bool mightBeFunctionDefinition() const {
    assert(MightBeFunctionDecl);
    // Try to determine if the end of a stream of tokens is either the
    // Definition or the Declaration for a function. It does this by looking for
    // the ';' in foo(); and using that it ends with a ; to know this is the
    // Definition, however the line could end with
    //    foo(); /* comment */
    // or
    //    foo(); // comment
    // or
    //    foo() // comment
    // endsWith() ignores the comment.
    return !endsWith(tok::semi);
  }

  /// \c true if this line starts a namespace definition.
  bool startsWithNamespace() const {
    return startsWith(tok::kw_namespace) || startsWith(TT_NamespaceMacro) ||
           startsWith(tok::kw_inline, tok::kw_namespace) ||
           startsWith(tok::kw_export, tok::kw_namespace);
  }

  /// TALLY : To state if there is a string literal in the line expression.
  bool hasStringLiteral() const {
      for (const FormatToken* curr = First; curr; curr = curr->getNextNonComment()) {
        if (curr->isStringLiteral())
          return true;
      }

      return false;
  }

  FormatToken *getFirstNonComment() const {
    assert(First);
    return First->is(tok::comment) ? First->getNextNonComment() : First;
  }

  FormatToken *getLastNonComment() const {
    assert(Last);
    return Last->is(tok::comment) ? Last->getPreviousNonComment() : Last;
  }

  FormatToken *First;
  FormatToken *Last;

  SmallVector<AnnotatedLine *, 0> Children;

  LineType Type;
  unsigned Level;
  unsigned PPLevel;
  size_t MatchingOpeningBlockLineIndex;
  size_t MatchingClosingBlockLineIndex;
  bool InPPDirective;
  bool InPragmaDirective;
  bool InMacroBody;
  bool MustBeDeclaration;
  bool MightBeFunctionDecl;
  bool IsMultiVariableDeclStmt;
  bool IsDeclaration = false;

  /// \c True if this line contains a macro call for which an expansion exists.
  bool ContainsMacroCall = false;

  /// \c True if this line should be formatted, i.e. intersects directly or
  /// indirectly with one of the input ranges.
  bool Affected;

  /// \c True if the leading empty lines of this line intersect with one of the
  /// input ranges.
  bool LeadingEmptyLinesAffected;

  /// \c True if one of this line's children intersects with an input range.
  bool ChildrenAffected;

  /// \c True if breaking after last attribute group in function return type.
  bool ReturnTypeWrapped;

  /// \c True if this line should be indented by ContinuationIndent in addition
  /// to the normal indention level.
  bool IsContinuation;

  unsigned FirstStartColumn;
  /// TALLY: \c True if this line is additional/double indented
  bool IsDoubleIndented;

  /// TALLY: Line state for columnarization
  unsigned LastSpecifierPadding = 0;
  unsigned LastSpecifierTabs = 0;

private:
  // Disallow copying.
  AnnotatedLine(const AnnotatedLine &) = delete;
  void operator=(const AnnotatedLine &) = delete;
};

/// Determines extra information about the tokens comprising an
/// \c UnwrappedLine.
class TokenAnnotator {
public:
  TokenAnnotator(const FormatStyle &Style, const AdditionalKeywords &Keywords)
      : Style(Style), Keywords(Keywords) {}

  /// TALLY: clean the set that is created.
  ~TokenAnnotator() {

      if (DefinedMacros.size() > 0)
          DefinedMacros.clear();
  }

  /// Adapts the indent levels of comment lines to the indent of the
  /// subsequent line.
  // FIXME: Can/should this be done in the UnwrappedLineParser?
  void setCommentLineLevels(SmallVectorImpl<AnnotatedLine *> &Lines) const;

  void annotate(AnnotatedLine &Line);
  void calculateFormattingInformation(AnnotatedLine &Line) const;

  /// TALLY: Add Tally-specific information to all annotated lines
  void calculateTallyInformation(AnnotatedLine &Line);

  /// TALLY: If a given token is part of a PP conditional inclusion
  bool IsPPConditionalInclusionScope = false;

  /// TALLY: If a given token is part of a struct scope
  bool IsStructScope = false;

  /// TALLY: If a given token is part of a union scope
  bool IsUnionScope = false;

  /// TALLY: If a given token is part of a class scope
  bool IsClassScope = false;

  /// TALLY: If a given token is part of a enum scope
  bool IsEnumScope = false;

  /// TALLY: If in function definition.
  bool IsInFunctionDefinition = false;

  /// TALLY : If in function definition Line. and not body.
  bool IsFunctionDefinitionLine = false;

  /// TALLY : If in template Line Basically in arrow braces inside expression of type. template <>.
  bool IsInTemplateLine = false;

  /// TALLY: Name of the struct (if any) a given token is scoped under
  StringRef StructScopeName = "<StructScopeName_None>";

  /// TALLY: Name of the class (if any) a given token is scoped under
  StringRef ClassScopeName = "<ClassScopeName_None>";

  /// TALLY: L-Brace count
  unsigned LbraceCount = 0;

  /// TALLY: R-Brace count
  unsigned RbraceCount = 0;

  /// TALLY: L-Paren count
  unsigned LparenCount = 0;

  /// TALLY: R-Paren count
  unsigned RparenCount = 0;
  
  /// TALLY: template opener count.
  unsigned LArrowCount = 0;

  /// TALLY: template closer count.
  unsigned RArrowCount = 0;

  /// TALLY: A weight to determine whether line break in the original must be enforced
  unsigned OriginalLineBreakWeight = 0;


private:
  /// Calculate the penalty for splitting before \c Tok.
  unsigned splitPenalty(const AnnotatedLine &Line, const FormatToken &Tok,
                        bool InFunctionDecl) const;

  bool spaceRequiredBeforeParens(const FormatToken &Right) const;

  bool spaceRequiredBetween(const AnnotatedLine &Line, const FormatToken &Left,
                            const FormatToken &Right) const;

  bool spaceRequiredBefore(const AnnotatedLine &Line,
                           const FormatToken &Right) const;

  bool mustBreakBefore(const AnnotatedLine &Line,
                       const FormatToken &Right) const;

  bool canBreakBefore(const AnnotatedLine &Line,
                      const FormatToken &Right) const;

  bool mustBreakForReturnType(const AnnotatedLine &Line) const;

  void printDebugInfo(const AnnotatedLine &Line) const;

  void calculateUnbreakableTailLengths(AnnotatedLine &Line) const;

  void calculateArrayInitializerColumnList(AnnotatedLine &Line) const;

  FormatToken *calculateInitializerColumnList(AnnotatedLine &Line,
                                              FormatToken *CurrentToken,
                                              unsigned Depth) const;
  FormatStyle::PointerAlignmentStyle
  getTokenReferenceAlignment(const FormatToken &PointerOrReference) const;

  FormatStyle::PointerAlignmentStyle getTokenPointerOrReferenceAlignment(
      const FormatToken &PointerOrReference) const;

  // TALLY
  void walkLine1(AnnotatedLine &Line);

  // TALLY
  void walkLine2(AnnotatedLine &Line);

  // TALLY
  bool CheckIfDatatype (FormatToken* token);

  // TALLY
  void MarkDatatype (FormatToken* token);

  // TALLY
  bool isFunctionLocalVariableDeclaration(const FormatToken * pTkn, const FormatToken * pNxt);

  // TALLY
  bool isValueAssignment(const FormatToken * pTkn);

  const FormatStyle &Style;

  const AdditionalKeywords &Keywords;

  vector<int> scopeCount = vector<int>(6, 0);
  SmallVector<ScopeType> Scopes;
  // TALLY: mark MACRO, is populated only when it is defined in same file it is used.
  static constexpr char STRDEFINETEXT[] {'d','e','f','i','n','e','\0'};

  set<string> DefinedMacros;
};

} // end namespace format
} // end namespace clang

#endif
