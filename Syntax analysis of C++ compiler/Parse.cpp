//Parse.cpp

/**
 * readme
 */


#include "Parse.h"
#include "ErrorsInfo.h"

/**
 * 构造，使用词法分析产生的token序列
 */
Parse::Parse ( vector<Token*> & v )
{
	tokens = v ;
	//这里为先前的token序列增加一个FILE_END来识别程序结束
	Token *temp = new Token( FILE_END , "" ) ; 
	tokens.push_back( temp ) ;
	token = tokens[0] ;
	iter = 1 ;
	root = NULL ;
	symtable = new SymTable ;
}

/**
 * 对外接口，返回值为整个程序的语法树
 */
TreeNode* Parse::program()
{
	declarations() ;		// 调用declarations函数分析变量的声明
	root = stmt_sequence();	// 调用stmt_sequence()分析语句列表， stmt_sequence()的返回值就是整个程序的语法树（不包括声明部分）
	return root ;
}

/**
 * 返回符号表
 */
SymTable * Parse::getsymTable()
{
	return symtable ;
}

/**
 * 后序遍历输出语法树
 */
void Parse::output()
{
	outputhelp( root , 0 ) ;
}

void Parse::outputhelp( TreeNode * subroot , int n )
{
	if( subroot == NULL ) return ;
	if( subroot->getChild(0) != NULL )
	{
		outputhelp( subroot->getChild(0) , n + 1 ) ;
		if( subroot->getChild(1) != NULL )
		{
			outputhelp( subroot->getChild(1) , n + 1 ) ;
			if( subroot->getChild(2) != NULL )
			{
				outputhelp( subroot->getChild(2) , n + 1 ) ;
			}
		}
	}
	for( int i = 0 ; i < n ; i ++ )
		cout << "  " ;
	cout << subroot->getNodeType() << endl ;
}



/**
 * 取下一个token，如果超出范围返回NULL
 */
Token* Parse::getToken()
{
	if( iter < tokens.size() )
		return tokens[iter ++] ;
	return NULL ;
}

/**
 * 匹配当前的token是否是我们期望的token
 * 如果是，那么然后取下一个token，返回true
 */
bool Parse::match( TypeKind expected )
{
	if( expected == token->getType() )
	{
		token = getToken() ;
		return true ;
	}
	else
	{
		//这里先不处理错误，把错误交给上级处理
		return false ;
	}
}


/**
 * 声明的分析函数
 * 把所有声明的数据存储起来
 */
void Parse::declarations()
{
	while( token->getType() == TK_INT || 
		token->getType() == TK_BOOL || 
		token->getType() == TK_CHAR ||
		token->getType() == TK_FLOAT ||
		token->getType() == TK_STRING )
	{
		Token *temptoken = token ; //保存类型,比如char ,int之类，然后
		token = getToken();	      //跳过类型说明符“int,bool,string”
		do{
			if( token->getType() == TK_COMMA ) 
				token = getToken() ; //循环回来时跳过当前这个逗号
			Token *tempid = token ;
			if( match(ID) ) 
			{
				Sym *sym = symtable->insertSym( tempid ) ; //插入符号表
				sym->setObjType(OT_VAR) ;
				switch( temptoken->getType() )
				{
				case TK_INT:
					sym->setValType(VT_INT) ;
					break;
				case TK_BOOL:
					sym->setValType(VT_BOOL) ;
					break;
				case TK_STRING:
					sym->setValType(VT_STRING) ;
					break;
				case TK_CHAR:
					sym->setValType(VT_CHAR) ;
					break;
				case TK_FLOAT:
					sym->setValType(VT_FLOAT) ;
					break;
				default:break;
				}
			}
		} while( token->getType() == TK_COMMA ) ;
		match(TK_SEMICOLON);	 // 匹配分号
	}
}


/**
 * 语句产生式
 * 这个函数分析单个语句下的所有语句,返回该语句根节点
 */
TreeNode * Parse::stmt_sequence()
{
	TreeNode *t1 = statement() ;
	//当没有碰到句子结束符，跳入循环
	while ( token->getType() != FILE_END && 
		(token->getType() != TK_END) && 
		(token->getType() != TK_ELSE) && 
		(token->getType() != TK_UNTIL))
	{
		TreeNode *t2 = NULL ;
		//匹配分好"；"
		//这里要千万注意：对于tiny的语法，最后一条赋值语句都是没有分号的；
		if ( ! match( TK_SEMICOLON ) ) 
		{
			errorsInfo.getErrorsInfo("lost semicolon!", 0) ;
			return NULL ; //errors! 
		}
		t2 = statement() ;
		if ( t2 != NULL ) 
			t1 = new TreeNode( STMT_SEQUENCE , t1 , t2 ) ;
	}
	return t1 ;
}


/**
 * 判断语句类型
 * 共有六类语句，其中赋值语句它是以ID开头的
 */
TreeNode * Parse::statement()
{
	TreeNode * t = NULL;
	switch ( token->getType() ) 
	{
	case TK_IF : 
		t = if_stmt(); 
		break;//如果当前的token是if语句；
	case TK_REPEAT : 
		t = repeat_stmt(); 
		break;//如果当前的token是repeat语句；
	case ID : 
		t = assign_stmt(); 
		break;//如果当前的token是赋值语句；
	case TK_READ : 
		t = read_stmt(); 
		break;//如果当前的token是read语句 ；
	case TK_WRITE : 
		t = write_stmt(); 
		break;//如果当前token是write语句；
	case TK_WHILE :
		t = while_stmt() ;
		break;//如果当前token是while语句；
	default : 
		//出错:
		errorsInfo.getErrorsInfo("statement errors!" , 0 ) ;
		return NULL ;
		break ;
	}
	return t ;
}


TreeNode * Parse::if_stmt()
{
	TreeNode * t , * cond_exp , *then_stmt , *else_stmt = NULL ;
	if( ! match(TK_IF) ) return NULL ; //errors!
	cond_exp = exp() ;
	if( ! match(TK_THEN) ) 
	{
		errorsInfo.getErrorsInfo( "lost then!" , 0 ) ;
		return NULL ; //errors!
	}
	then_stmt = stmt_sequence() ;
	if( token->getType() == TK_ELSE ) 
	{
		if( ! match(TK_ELSE) ) return NULL ; //errors!
		else_stmt = stmt_sequence() ;
	}
	if( ! match(TK_END) ) return NULL ; //errors!
	t = new TreeNode(IF_STMT, cond_exp, then_stmt, else_stmt) ;
	return t ;
}

TreeNode * Parse::repeat_stmt()
{
	TreeNode * t , * rep_stmt , * cond_exp ;
	if( ! match( TK_REPEAT ) ) return NULL ; //errors!
	rep_stmt = stmt_sequence() ;
	if( ! match( TK_UNTIL ) ) 
	{
		errorsInfo.getErrorsInfo("lost until!" , 0 ) ;
		return NULL ; //errors!
	}
	cond_exp = exp() ;
	t = new TreeNode( REPEAT_STMT , rep_stmt , cond_exp ) ;
	return t ;
}

TreeNode * Parse::assign_stmt()
{
	TreeNode * t , * cond_exp ;
	Token *temptoken ;
	Sym * tempSym ;
	if( token->getType() == ID ) //TODO:分配
	{
		tempSym = symtable->fineSym(token) ;
		if( tempSym == NULL )
		{
			errorsInfo.getErrorsInfo("undefine id", 0) ;
			return NULL ;
		}
		temptoken = token ;
	}
	if( ! match(ID) ) return NULL ; //errors!
	if( ! match(TK_ASSIGN) ) 
	{
		errorsInfo.getErrorsInfo("assign errors!" , 0 ) ;
		return NULL ; //errors!
	}
	cond_exp = exp() ;
	t = new TreeNode( ASSIGN_STMT , cond_exp ) ;
	t->setToken(temptoken) ;
	t->setValType(tempSym->getValType()) ;
	return t ;
}

TreeNode * Parse::read_stmt()
{
	TreeNode * t ;
	Token *temptoken ;
	Sym * tempSym ;
	if( ! match(TK_READ) ) return NULL ; //errors!
	if( token->getType() == ID ) //TODO:分配
	{
		tempSym = symtable->fineSym(token) ;
		if( tempSym == NULL )
		{
			errorsInfo.getErrorsInfo("undefine id", 0) ;
			return NULL ;
		}
		temptoken = token ;
	}
	if( ! match(ID) ) 
	{
		errorsInfo.getErrorsInfo( "read errors!" , 0 ) ;
		return NULL ; //errors!
	}
	t = new TreeNode( READ_STMT ) ;
	t->setToken( temptoken ) ;
	t->setValType( tempSym->getValType() ) ;
	return t ;
}

TreeNode * Parse::write_stmt() 
{
	TreeNode * t , *cond_exp ;
	if( ! match(TK_WRITE) ) return NULL ; //errors!
	cond_exp = exp() ;
	t = new TreeNode( WRITE_STMT , cond_exp ) ;
	return t ;
}

TreeNode * Parse::while_stmt()
{
	TreeNode *t , *cond_exp , *rep_stmt ;
	if( ! match(TK_WHILE) ) return NULL ; //errors!
	cond_exp = exp() ;
	if( ! match(TK_DO) ) 
	{
		errorsInfo.getErrorsInfo("lost do" , 0) ;
		return NULL ; //errors!
	}
	rep_stmt = stmt_sequence() ;
	if( ! match(TK_END) ) 
	{
		errorsInfo.getErrorsInfo("lost end" , 0) ;
		return NULL ; //errors!
	}
	t = new TreeNode( WHILE_STMT , cond_exp , rep_stmt ) ;
	return t ;
}

/**
 * 运算的匹配
 * 递归地匹配，最后匹配逻辑与或非
 */
TreeNode * Parse::exp()
{
	TreeNode * t1 = comparison_exp() ;
	if((token->getType() == TK_AND ) || (token->getType() == TK_OR) ||
		(token->getType() == TK_NOT)) 
	{
		TreeNode * t2 ;
		TypeKind typekind = token->getType() ;
		token = getToken() ;
		t2 = comparison_exp() ;
		switch( typekind )
		{
		case TK_AND:
			t1 = new TreeNode( LOG_AND_EXP , t1 , t2 ) ;
			break;
		case TK_OR:
			t1 = new TreeNode( LOG_OR_EXP , t1 , t2 ) ;
			break;
		case TK_NOT:
			t1 = new TreeNode( LOG_NOT_EXP , t2 ) ;
			break;
		default:
			cout << "errors!" << endl ;
			break ;
		}
	}
	return t1 ;
}

/**
 * 匹配等号与不等号
 */
TreeNode * Parse::comparison_exp()
{
	TreeNode * t1 = arithmetic_exp() ;
	if((token->getType() == TK_GTR ) || (token->getType() == TK_GEQ) ||
		(token->getType() == TK_LSS) || (token->getType() == TK_LEQ) ||
		(token->getType() == TK_EQU)) 
	{
		TreeNode * t2 ;
		TypeKind typekind = token->getType() ;
		token = getToken() ;
		t2 = arithmetic_exp() ;
		switch( typekind )
		{
		case TK_GTR:
			t1 = new TreeNode( GTR_EXP , t1 , t2 ) ;
			break;
		case TK_GEQ:
			t1 = new TreeNode( GEQ_EXP , t1 , t2 ) ;
			break;
		case TK_LSS:
			t1 = new TreeNode( LSS_EXP , t1 , t2 ) ;
			break;
		case TK_LEQ:
			t1 = new TreeNode( LEQ_EXP , t1 , t2 ) ;
			break;
		case TK_EQU:
			t1 = new TreeNode( EQU_EXP , t1 , t2 ) ;
			break;
		}
	}
	return t1 ;
}

/**
 * 四则运算
 * 匹配加减
 */
TreeNode * Parse::arithmetic_exp()
{ 
	TreeNode * t1 = simple_exp();
	while ((token->getType() == TK_ADD) || (token->getType() == TK_SUB))
	{ 
		TreeNode * t2 ;
		TypeKind typekind = token->getType() ;
		token = getToken() ;
		t2 = simple_exp() ;
		if( typekind == TK_ADD )
			t1 = new TreeNode( ADD_EXP , t1 , t2 ) ;
		else t1 = new TreeNode( SUB_EXP , t1 , t2 ) ;
	}
	return t1 ;
}

/**
 * 四则运算
 * 匹配乘除
 */
TreeNode * Parse::simple_exp()
{ 
	TreeNode * t1 = factor() ;
	while ((token->getType() == TK_MUL ) || (token->getType() == TK_DIV))
	{ 
		TreeNode *t2 ;
		TypeKind typekind = token->getType() ;
		token = getToken() ;
		t2 = factor() ;
		if( typekind == TK_MUL )
			t1 = new TreeNode( MUL_EXP , t1 , t2 ) ;
		else t1 = new TreeNode( DIV_EXP , t1 , t2 ) ;
	}
	return t1 ;
}

TreeNode * Parse::factor()
{
	TreeNode * t ;
	Sym * tempSym ;
	switch( token->getType() )
	{
	case NUM:
		t = new TreeNode( FACTOR ) ;
		t->setToken(token) ;
		t->setValType(VT_INT) ;
		if( ! match(NUM) ) return NULL ; //errors!
		break;
	case ID:
		tempSym = symtable->fineSym(token) ;
		if( tempSym == NULL )
		{
			errorsInfo.getErrorsInfo("undefine id" , 0);
			return NULL ;
		}
		t = new TreeNode( FACTOR ) ;
		t->setToken( token ) ;
		t->setValType( tempSym->getValType() ) ;
		if( ! match(ID) ) return NULL ; //errors!
		break;
	case TK_LP:
		if( ! match(TK_LP) ) return NULL ; //errors!
		t = exp() ;
		if( ! match(TK_RP) ) 
		{
			errorsInfo.getErrorsInfo("lost )" , 0) ;
			return NULL ; //errors!
		}
		break;
	default:
		//errors!
		errorsInfo.getErrorsInfo("errors!" , 0) ;
		return NULL ;
		break;
	}
	return t ;
}