#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"


using namespace clang;

class CountImplicitCastVisitor : public RecursiveASTVisitor<CountImplicitCastVisitor> {
public:
    explicit CountImplicitCastVisitor(ASTContext * Context) : Context(Context){}

    // отдельно, чтобы выводил глобальные
    bool VisitVarDecl(VarDecl *Var) {
        if (CurrentGlobVar) {
            llvm::errs() << "\n";
        }
        if (Var->hasGlobalStorage()) {
            CurrentGlobVar = Var;
            llvm::errs() << "Global var:\n";
        }
        return true;
    }

    // проход по функциям, чтобы красиво выводить названия
    bool VisitFunctionDecl(FunctionDecl *Func) {
        if (CurrentFunction) {
            llvm::errs() << "\n";
        }
        if (Func->hasBody()) {
            CurrentFunction = Func;
            llvm::errs() << "Function `" << Func->getNameAsString() << "`:\n";
        }
        return true;
    }

    // проход уже по самим implicit cast и вывод тех, которые подходят под ТЗ
    bool VisitImplicitCastExpr(ImplicitCastExpr *Cast) {
        QualType SrcType = Cast->getSubExpr()->getType();
        QualType DstType = Cast->getType();

        // нужны именно неявные, а не (double)3.14f и убираем ситуации LValueToRValue
        if (Cast->isPartOfExplicitCast() || SrcType.getCanonicalType() == DstType.getCanonicalType()) {
            return true;
        }

        count++;
        std::string kindName = Cast->getCastKindName();
        llvm::errs() << "Kind: " << kindName << ": " << SrcType.getAsString() << " -> " << DstType.getAsString() << "\n";
        return true;
    }

    // Для Consumer - вывести суммарное кол-во неявных кастов
    int getCount() const {
        return count;
    }

private:
    ASTContext *Context;
    FunctionDecl *CurrentFunction = nullptr;
    VarDecl *CurrentGlobVar = nullptr;
    int count = 0;
};

class CountImplicitCastConsumer : public ASTConsumer {
public:
    explicit CountImplicitCastConsumer(ASTContext *Context) : Visitor(Context) {}
    void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        llvm::errs() << "-------------------------\n";
        llvm::errs() << "Total implicit casts: " << Visitor.getCount() << "\n";
    }
private:
    CountImplicitCastVisitor Visitor;
};

class CountImplicitCastAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &Compiler, llvm::StringRef InFile) override {
        return std::make_unique<CountImplicitCastConsumer>(&Compiler.getASTContext());
    }
    bool ParseArgs(const CompilerInstance &Compiler, const std::vector<std::string> &args) override {
        return true;
    }

    ActionType getActionType() override {
        return AddAfterMainAction;
    }
};

static FrontendPluginRegistry::Add<CountImplicitCastAction>
X("implicit-cast-counter", "Plugin for counting implicit casts");