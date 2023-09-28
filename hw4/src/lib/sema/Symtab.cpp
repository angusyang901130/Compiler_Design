#include "sema/Symtab.hpp"

SymbolEntry::SymbolEntry(const char* name, const char* kind, int level, 
                const char* type, AstNode* attr)
                : name(name), kind(kind), level(level), type(type){
    
    this->attr = attr;
}

const char* SymbolEntry::getName(){
    return this->name.c_str();
}

const char* SymbolEntry::getKind(){
    return this->kind.c_str();
}

int SymbolEntry::getLevel(){
    return this->level;
}

const char* SymbolEntry::getType(){
    return this->type.c_str();
}

AstNode* SymbolEntry::getAttr(){
    return this->attr;
}

SymbolTable::SymbolTable(){}

void SymbolTable::addSymbol(const char* name, const char* kind, int level, const char* type, AstNode* attr){
    SymbolEntry new_entry = SymbolEntry(name, kind, level, type, attr);
    this->entries.push_back(new_entry);
}

void SymbolTable::dumpDemarcation(const char chr) {
    for (size_t i = 0; i < 110; ++i) {
        printf("%c", chr);
    }
    puts("");
}

void SymbolTable::dumpSymbol() {
    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                            "Attribute");
    dumpDemarcation('-');
    
    for(SymbolEntry sym_ent : this->entries){
        printf("%-33s", sym_ent.getName());

        const char* kind = sym_ent.getKind();
        printf("%-11s", kind);

        if(sym_ent.getLevel() == 0)
            printf("%d%-10s", 0, "(global)");
        else
            printf("%d%-10s", sym_ent.getLevel(), "(local)");
        
        printf("%-17s", sym_ent.getType());
        
        if(strcmp(kind, "function") == 0){
            AstNode* attr = sym_ent.getAttr();
            FunctionNode* func = dynamic_cast<FunctionNode*>(attr); 
            printf("%-11s", func->getParamType());
        }else if(strcmp(kind, "constant") == 0){
            AstNode* attr = sym_ent.getAttr();
            ConstantValueNode* cons = dynamic_cast<ConstantValueNode*>(attr);
            const char* const_type = cons->getType();
            ConstValue value = cons->getValue();

            if(strcmp(const_type, "integer") == 0){
                printf("%-11d", value.int_value);
            }else if(strcmp(const_type, "real") == 0){
                printf("%-11f", value.real_value);
            }else if(strcmp(const_type, "boolean") == 0){
                if(value.bool_value)
                    printf("%-11s", "true");
                else printf("%-11s", "false");
            }else if(strcmp(const_type, "string") == 0){
                printf("%-11s", value.str_value);
            }
        }
            

        puts("");
    }
    
    
    dumpDemarcation('-');
}

SymbolTable* SymbolManager::getCurSymTab(){
    return this->st_tables.top();
}

std::vector<SymbolEntry>& SymbolTable::getEntries(){
    return this->entries;
}

SymbolManager::SymbolManager(){}

void SymbolManager::pushScope(SymbolTable *new_scope){
    this->st_tables.push(new_scope);
    this->vec_tables.push_back(new_scope);
    this->level += 1;
}

void SymbolManager::popScope(){
    SymbolTable* symtab = this->st_tables.top();
    this->vec_tables.pop_back();

    // printf("opt_sym: %d\n", opt_sym);
    if(opt_sym)
        symtab->dumpSymbol();

    this->st_tables.pop();
    this->level -= 1;
}

int SymbolManager::getCurLevel(){
    return this->level;
}

const char* SymbolManager::getFunctInvocType(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getKind(), "function") == 0){
                AstNode* attr = entry.getAttr();
                FunctionNode* funct = dynamic_cast<FunctionNode*>(attr);
                return funct->getReturnType();
            }
        }
    }
}

void SymbolManager::setVarRefType(VariableReferenceNode& p_var_ref){

    char* type;

    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), p_var_ref.getRefName()) == 0){
                type = strdup(entry.getType());
            }
        }
    }

    if(strcmp(p_var_ref.getType(), "array") != 0){
        p_var_ref.setType(type);
        return;
    }

    int dim_idx = p_var_ref.getExprs().size();
    std::vector<char*> dim;

    char* ref_type = (char*)calloc(50, sizeof(char));

    char* scalar_type = strtok(type, "[");

    char* dim_tok = strtok(NULL, "[");

    while(dim_tok != NULL){
        char* new_dim_tok = strdup(dim_tok);
        dim.push_back(new_dim_tok);
        dim_tok = strtok(NULL, "[");
    }

    strcpy(ref_type, scalar_type);

    if(dim_idx == dim.size()){
        ref_type[strlen(scalar_type)-1] = 0;
        p_var_ref.setType(ref_type);
        return;
    }

    for(int i = dim_idx; i < dim.size(); i++){
        strcat(ref_type, "[");
        strcat(ref_type, dim[i]);
    }

    p_var_ref.setType(ref_type);
    return;
}

const char* SymbolManager::getFunctParamType(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), name) == 0){
                FunctionNode* funct = dynamic_cast<FunctionNode*>(entry.getAttr());
                // printf("get funct: %s\n", funct->getParamType());
                return funct->getParamType();
            }
        }
    }

    return NULL;
}

const char* SymbolManager::getFunctReturnType(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), name) == 0){
                FunctionNode* funct = dynamic_cast<FunctionNode*>(entry.getAttr());
                // printf("get funct: %s\n", funct->getParamType());
                return entry.getType();
            }
        }
    }

    return NULL;
}

void SymbolManager::setReturnType(ReturnNode& return_node){

    if(this->funct_stack.empty()){
        return_node.setReturnType("program-error");
        return;
    }

    // printf("current: %s\n", this->funct_stack.top());
    
    for(int i = this->vec_tables.size()-1; i >= 0; i--){
        std::vector<SymbolEntry> entries = this->vec_tables[i]->getEntries();
        for(int j = entries.size()-1; j >= 0; j--){
            if(strcmp(entries[j].getKind(), "function") == 0 && strcmp(this->funct_stack.top(), entries[j].getName()) == 0){
                //printf("funct name: %s, return type: %s\n", entries[j].getName(), entries[j].getType());
                return_node.setReturnType(strdup(entries[j].getType()));
                // this->funct_stack.pop();
                // printf("found ret: %d\n", entries[j].getFoundRet());
                return;
            }
        }
    }
}

void SymbolManager::pushFunctStack(const char* name){
    this->funct_stack.push(name);
}

void SymbolManager::popFunctStack(){
    this->funct_stack.pop();
}

bool SymbolManager::isFunctStackEmpty(){
    return this->funct_stack.empty();
}

bool SymbolManager::isRedecl(const char* name){

    SymbolTable* cur_symtab = this->getCurSymTab();

    for(SymbolEntry entry : cur_symtab->getEntries()){
        // printf("entry name: %s\n", entry.getName());
        // printf("name: %s\n", name);
        if(strcmp(entry.getName(), name) == 0){
            return true;
        }
    }

    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getKind(), "loop_var") == 0){
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::isErrorDim(const char* type){

    if(strcmp(type, "integer") == 0 || strcmp(type, "real") == 0 || strcmp(type, "boolean") == 0 
                || strcmp(type, "string") == 0){
        
        return false;
    }

    char* arr_type = (char*)calloc(50, sizeof(char));
    strcpy(arr_type, type);

    char* tok = strtok(arr_type, "[");
    tok = strtok(NULL, "[");

    while(tok != NULL){
        char* ptr;
        long dim_val = strtol(tok, &ptr, 10);

        if(dim_val <= 0){
            return true;
        }
        tok = strtok(NULL, "[");
    }

    return false;
}

bool SymbolManager::symNotFound(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // printf("entry: %s\n", entry.getName());
            if(strcmp(entry.getName(), name) == 0){
                return false;
            }
        }
    }
    return true;
}

bool SymbolManager::symNotVar(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            if(strcmp(entry.getName(), name) == 0 && (strcmp(entry.getKind(), "variable") == 0 || strcmp(entry.getKind(), "constant") == 0 || 
                    strcmp(entry.getKind(), "loop_var") == 0)){
                return false;
            }
        }
    }
    return true;
}

bool SymbolManager::dimNotInt(VariableReferenceNode& var_ref){
    if(var_ref.exprNotInt())
        return true;
    else return false;
}

bool SymbolManager::overArrSubScript(VariableReferenceNode& var_ref){
    
    int dim_size = var_ref.getExprs().size();
    // const char* ref_name = strdup(var_ref.getRefName());

    // printf("var_ref name: %s\n", var_ref.getRefName());

    char* sym_type;

    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            if(strcmp(entry.getName(), var_ref.getRefName()) == 0 && strcmp(entry.getKind(), "variable") == 0){
                // printf("entry name: %s\n", entry.getName());
                sym_type = strdup(entry.getType());
                // printf("sym_type: %s\n", sym_type);
                break;
            }
        }
    }

    // printf("dim_size: %d\n", dim_size);
    char* arr_type = strdup(sym_type);
    // strcpy(arr_type, sym_type);
    // printf("sym_type: %s\n", sym_type);
    // printf("arr type: %s\n", arr_type);

    char* tok = strtok(arr_type, "[");
    tok = strtok(NULL, "[");

    int cnt = 0;

    while(tok != NULL){
        // printf("partition: %s\n", tok);
        cnt++;
        tok = strtok(NULL, "[");
    }

    // printf("cnt: %d\n", cnt);

    if(dim_size > cnt)
        return true;
    else return false;
}

bool SymbolManager::invalidBinOp(BinaryOperatorNode& bin_op){

    if(strcmp(bin_op.getBackExpr()->getType(), "error-type") == 0){
        return false;
    }else if(strcmp(bin_op.getFrontExpr()->getType(), bin_op.getBackExpr()->getType()) == 0 && strcmp("string", bin_op.getFrontExpr()->getType()) == 0){

        if(strcmp(bin_op.getOp(), "+") == 0)
            return false;

    }else if((strcmp(bin_op.getOp(), "and") == 0 || strcmp(bin_op.getOp(), "or") == 0) && 
            (strcmp(bin_op.getFrontExpr()->getType(), "boolean") != 0 || strcmp(bin_op.getFrontExpr()->getType(), "boolean") != 0)){

        return true;

    }else if(strcmp(bin_op.getOp(), "mod") == 0 && (strcmp(bin_op.getFrontExpr()->getType(), "integer") != 0 || 
                strcmp(bin_op.getBackExpr()->getType(), "integer") != 0)){
        
        return true;

    }else if(strcmp(bin_op.getFrontExpr()->getType(), bin_op.getBackExpr()->getType()) == 0){

        return false;

    }else{
        if(strcmp(bin_op.getOp(), "and") != 0 && strcmp(bin_op.getOp(), "mod") != 0 && strcmp(bin_op.getOp(), "or") != 0){

            if((strcmp(bin_op.getFrontExpr()->getType(), "integer") == 0 || strcmp(bin_op.getFrontExpr()->getType(), "real") == 0) && 
                (strcmp(bin_op.getBackExpr()->getType(), "integer") == 0 || strcmp(bin_op.getBackExpr()->getType(), "real") == 0)){
                
                return false;
            }
        }
    }

    return true;
}

bool SymbolManager::invalidUnOp(UnaryOperatorNode& un_op){

    if(strcmp(un_op.getExpr()->getType(), "error-type") == 0){
        return false;
    }else if(strcmp(un_op.getExpr()->getType(), "boolean") != 0 && strcmp(un_op.getOp(), "not") == 0){
        return true;
    }else if((strcmp(un_op.getExpr()->getType(), "integer") == 0 || strcmp(un_op.getExpr()->getType(), "real") == 0) && strcmp(un_op.getOp(), "neg") == 0){
        return false;
    }else if(strcmp(un_op.getOp(), "neg") == 0){
        return true;
    }
    
    return false;

}

bool SymbolManager::isFunctDecl(const char* name){

    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            if(strcmp(entry.getName(), name) == 0){
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::isSymFunct(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getKind(), "function") == 0){
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::invalidArgNum(int param_num, FunctionInvocationNode& funct_invoc){

    char* sym_param_type = (char*)funct_invoc.getParamType();

    int sym_type_num = 0;
    char* tmp_sym_param_type = strdup(sym_param_type);
    // printf("sym_param_type: %s\n", sym_param_type);
    char* sym_param = strtok(tmp_sym_param_type, ",");

    while(sym_param != NULL){
        // printf("type: %s\n", sym_param);
        sym_type_num++;
    
        sym_param = strtok(NULL, ",");
    }

    // printf("sym_type_num: %d, param_num: %d\n", sym_type_num, param_num);

    if(sym_type_num != param_num)
        return true;
    else return false;
}

bool SymbolManager::argTypeNotMatch(FunctionInvocationNode& funct_invoc){

    std::vector<ExpressionNode*> exprs = funct_invoc.getExprs();
    std::vector<char*> type_vec;

    for(ExpressionNode* expr : exprs){
        type_vec.push_back(strdup(expr->getType()));
    }

    char* sym_param_type = (char*)funct_invoc.getParamType();

    std::vector<char*> sym_type_vec;

    char* tmp_sym_param_type = strdup(sym_param_type);
    char* sym_param = strtok(tmp_sym_param_type, ",");

    bool first = true;
    while(sym_param != NULL){
        char* new_sym_param;

        if(first){
            new_sym_param = strdup(sym_param);
            first = false;
        }else new_sym_param = strdup(sym_param+1);
        
        // printf("sym_param: %s\n", new_sym_param);
        sym_type_vec.push_back(new_sym_param);
        sym_param = strtok(NULL, ",");
    }

    for(int i = 0; i < type_vec.size(); i++){
        if(strcmp(sym_type_vec[i], type_vec[i]) != 0){
            return true;
        }
    }

    return false;
}

bool SymbolManager::isNotScalar(const char* type){
    if(strcmp(type, "integer") == 0 || strcmp(type, "real") == 0 || strcmp(type, "boolean") == 0 || strcmp(type, "string") == 0)
        return false;
    else return true;
}

bool SymbolManager::isLoopVarOrContant(const char* name){

    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            if(strcmp(entry.getName(), name) == 0 && (strcmp(entry.getKind(), "loop_var") == 0 || strcmp(entry.getKind(), "constant") == 0)){
                // printf("entry name: %s\n", entry.getName());
                // printf("sym_type: %s\n", sym_type);
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::isArray(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getType(), "integer") != 0 && strcmp(entry.getType(), "real") != 0 &&
                    strcmp(entry.getType(), "string") != 0 && strcmp(entry.getType(), "boolean") != 0){
                // printf("entry name: %s\n", entry.getName());
                // printf("sym_type: %s\n", sym_type);
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::isConst(const char* name){
    // printf("name: %s\n", name);
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getKind(), "constant") == 0){
                // printf("entry name: %s\n", entry.getName());
                // printf("entry kind: %s\n", entry.getKind());
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::isLoopVar(const char* name){
    for(SymbolTable* symtab : this->vec_tables){
        for(SymbolEntry entry : symtab->getEntries()){
            // const char* tmp_name = strdup(entry.getName());
            
            if(strcmp(entry.getName(), name) == 0 && strcmp(entry.getKind(), "loop_var") == 0 && this->getCurLevel() > entry.getLevel()){
                // printf("entry name: %s\n", entry.getName());
                // printf("entry kind: %s\n", entry.getKind());
                return true;
            }
        }
    }

    return false;
}

bool SymbolManager::checkAssignType(AssignmentNode* assign_node){
    char* var_ref_type = strdup(assign_node->getVarRef()->getType());
    char* expr_type = strdup(assign_node->getExpr()->getType());

    if(strcmp(var_ref_type, expr_type) == 0)
        return true;
    else if(strcmp(var_ref_type, "real") == 0 && strcmp(expr_type, "integer") == 0)
        return true;
    else return false;
}

bool SymbolManager::checkExprError(std::vector<ExpressionNode*>& exprs){
    for(ExpressionNode* expr : exprs){
        if(strcmp(expr->getType(), "error-type") == 0)
            return true;
    }

    return false;
}