//Parse.cpp

/**
 * readme
 */


#include "Parse.h"
#include "ErrorsInfo.h"

/**
 * ���죬ʹ�ôʷ�����������token����
 */
Parse::Parse ( vector<Token*> & v )
{
	tokens = v ;
	//����Ϊ��ǰ��token��������һ��FILE_END��ʶ��������
	Token *temp = new Token( FILE_END , "" ) ; 
	tokens.push_back( temp ) ;
	token = tokens[0] ;
	iter = 1 ;
	root = NULL ;
	symtable = new SymTable ;
}

/**
 * ����ӿڣ�����ֵΪ����������﷨��
 */
TreeNode* Parse::program()
{
	declarations() ;		// ����declarations������������������
	root = stmt_sequence();	// ����stmt_sequence()��������б� stmt_sequence()�ķ���ֵ��������������﷨�����������������֣�
	return root ;
}

/**
 * ���ط��ű�
 */
SymTable * Parse::getsymTable()
{
	return symtable ;
}

/**
 * �����������﷨��
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
 * ȡ��һ��token�����������Χ����NULL
 */
Token* Parse::getToken()
{
	if( iter < tokens.size() )
		return tokens[iter ++] ;
	return NULL ;
}

/**
 * ƥ�䵱ǰ��token�Ƿ�������������token
 * ����ǣ���ôȻ��ȡ��һ��token������true
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
		//�����Ȳ�������󣬰Ѵ��󽻸��ϼ�����
		return false ;
	}
}


/**
 * �����ķ�������
 * ���������������ݴ洢����
 */
void Parse::declarations()
{
	while( token->getType() == TK_INT || 
		token->getType() == TK_BOOL || 
		token->getType() == TK_CHAR ||
		token->getType() == TK_FLOAT ||
		token->getType() == TK_STRING )
	{
		Token *temptoken = token ; //��������,����char ,int֮�࣬Ȼ��
		token = getToken();	      //��������˵������int,bool,string��
		do{
			if( token->getType() == TK_COMMA ) 
				token = getToken() ; //ѭ������ʱ������ǰ�������
			Token *tempid = token ;
			if( match(ID) ) 
			{
				Sym *sym = symtable->insertSym( tempid ) ; //������ű�
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
		match(TK_SEMICOLON);	 // ƥ��ֺ�
	}
}


/**
 * ������ʽ
 * �������������������µ��������,���ظ������ڵ�
 */
TreeNode * Parse::stmt_sequence()
{
	TreeNode *t1 = statement() ;
	//��û���������ӽ�����������ѭ��
	while ( token->getType() != FILE_END && 
		(token->getType() != TK_END) && 
		(token->getType() != TK_ELSE) && 
		(token->getType() != TK_UNTIL))
	{
		TreeNode *t2 = NULL ;
		//ƥ��ֺ�"��"
		//����Ҫǧ��ע�⣺����tiny���﷨�����һ����ֵ��䶼��û�зֺŵģ�
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
 * �ж��������
 * ����������䣬���и�ֵ���������ID��ͷ��
 */
TreeNode * Parse::statement()
{
	TreeNode * t = NULL;
	switch ( token->getType() ) 
	{
	case TK_IF : 
		t = if_stmt(); 
		break;//�����ǰ��token��if��䣻
	case TK_REPEAT : 
		t = repeat_stmt(); 
		break;//�����ǰ��token��repeat��䣻
	case ID : 
		t = assign_stmt(); 
		break;//�����ǰ��token�Ǹ�ֵ��䣻
	case TK_READ : 
		t = read_stmt(); 
		break;//�����ǰ��token��read��� ��
	case TK_WRITE : 
		t = write_stmt(); 
		break;//�����ǰtoken��write��䣻
	case TK_WHILE :
		t = while_stmt() ;
		break;//�����ǰtoken��while��䣻
	default : 
		//����:
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
	if( token->getType() == ID ) //TODO:����
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
	if( token->getType() == ID ) //TODO:����
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
 * �����ƥ��
 * �ݹ��ƥ�䣬���ƥ���߼�����
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
 * ƥ��Ⱥ��벻�Ⱥ�
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
 * ��������
 * ƥ��Ӽ�
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
 * ��������
 * ƥ��˳�
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