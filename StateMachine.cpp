#include "StateMachine.h"

StateMachineClass::StateMachineClass()
	:mCurrentState(START_STATE)
{
	for(int i=0; i<LAST_STATE; i++) {
		for(int j=0; j<LAST_CHAR; j++) {
			mLegalMoves[i][j] = CANTMOVE_STATE;
		}
	}
	for(int k=0; k<LAST_CHAR; k++) {
		mLegalMoves[LINECOMMENT_STATE][k] = LINECOMMENT_STATE;
		mLegalMoves[BLOCKCOM1_STATE][k] = BLOCKCOM1_STATE;
		mLegalMoves[BLOCKCOM2_STATE][k] = BLOCKCOM1_STATE;
	}
	//whitespace
	mLegalMoves[START_STATE][NEWLINE_CHAR]= START_STATE;
	mLegalMoves[START_STATE][WHITESPACE_CHAR]= START_STATE;
	//integer
	mLegalMoves[START_STATE][DIGIT_CHAR]= INTEGER_STATE;
	mLegalMoves[INTEGER_STATE][DIGIT_CHAR]= INTEGER_STATE;
	//mLegalMoves[INTEGER_STATE][LESS_CHAR]= CANTMOVE_STATE;  redundant?
	//indentifier
	mLegalMoves[START_STATE][LETTER_CHAR]= IDENTIFIER_STATE;
	mLegalMoves[IDENTIFIER_STATE][LETTER_CHAR]= IDENTIFIER_STATE;
	mLegalMoves[IDENTIFIER_STATE][DIGIT_CHAR]= IDENTIFIER_STATE;
	mLegalMoves[IDENTIFIER_STATE][UNDERSCORE_CHAR]= IDENTIFIER_STATE;
	//asgn , (less/greater)equality
	mLegalMoves[START_STATE][EQUAL_CHAR]= ASSIGNMENT_STATE;
	mLegalMoves[ASSIGNMENT_STATE][EQUAL_CHAR]= EQUAL_STATE;
	mLegalMoves[ASSIGNMENT_STATE][LESS_CHAR]= LESSEQUAL_STATE;
	mLegalMoves[ASSIGNMENT_STATE][GREATER_CHAR]= GREATEREQUAL_STATE;
	// less(equality) / insertion
	mLegalMoves[START_STATE][LESS_CHAR]= LESS_STATE;
	mLegalMoves[LESS_STATE][LESS_CHAR]= INSERTION_STATE;
	mLegalMoves[LESS_STATE][EQUAL_CHAR]= LESSEQUAL_STATE;
	// greater(equal)
	mLegalMoves[START_STATE][GREATER_CHAR]= GREATER_STATE;
	mLegalMoves[GREATER_STATE][EQUAL_CHAR]= GREATEREQUAL_STATE;
	// or
	mLegalMoves[START_STATE][OR_CHAR]= HO_STATE;
	mLegalMoves[HO_STATE][OR_CHAR]= OR_STATE;
	// and
	mLegalMoves[START_STATE][AND_CHAR]= HA_STATE;
	mLegalMoves[HA_STATE][AND_CHAR]= AND_STATE;
	//not
	mLegalMoves[START_STATE][NOT_CHAR]= NOT_STATE;
	// paren, curly
	mLegalMoves[START_STATE][RPAREN_CHAR]= RPAREN_STATE;
	mLegalMoves[START_STATE][LPAREN_CHAR]= LPAREN_STATE;
	mLegalMoves[START_STATE][RCURLY_CHAR]= RCURLY_STATE;
	mLegalMoves[START_STATE][LCURLY_CHAR]= LCURLY_STATE;
	// math ops
	mLegalMoves[START_STATE][PLUS_CHAR]= PLUS_STATE;
	mLegalMoves[START_STATE][MINUS_CHAR]= MINUS_STATE;
	mLegalMoves[START_STATE][TIMES_CHAR]= TIMES_STATE;
	mLegalMoves[START_STATE][DIVIDE_CHAR]= DIVIDE_STATE;
	//comments
	mLegalMoves[DIVIDE_STATE][DIVIDE_CHAR] = LINECOMMENT_STATE;
	mLegalMoves[DIVIDE_STATE][TIMES_CHAR] = BLOCKCOM1_STATE;
	mLegalMoves[BLOCKCOM1_STATE][TIMES_CHAR] = BLOCKCOM2_STATE;
	mLegalMoves[BLOCKCOM2_STATE][TIMES_CHAR] = BLOCKCOM2_STATE;
	mLegalMoves[BLOCKCOM2_STATE][DIVIDE_CHAR] = BLOCKCOM3_STATE;
	mLegalMoves[LINECOMMENT_STATE][NEWLINE_CHAR] = CANTMOVE_STATE;
	// etc
	mLegalMoves[START_STATE][SEMICOLON_CHAR]= SEMICOLON_STATE;
	mLegalMoves[START_STATE][ENDFILE_CHAR]= ENDFILE_STATE;

	

	for(int i=0; i<LAST_STATE; i++) {
		mCorrespondingTokenTypes[i]=BAD_TOKEN;
	}
	mCorrespondingTokenTypes[IDENTIFIER_STATE] = IDENTIFIER_TOKEN;
	mCorrespondingTokenTypes[INTEGER_STATE] = INTEGER_TOKEN;
	mCorrespondingTokenTypes[INSERTION_STATE] = INSERTION_TOKEN;
	mCorrespondingTokenTypes[LESS_STATE] = LESS_TOKEN;
	mCorrespondingTokenTypes[LESSEQUAL_STATE] = LESSEQUAL_TOKEN;
	mCorrespondingTokenTypes[GREATER_STATE] = GREATER_TOKEN;
	mCorrespondingTokenTypes[GREATEREQUAL_STATE] = GREATEREQUAL_TOKEN;
	mCorrespondingTokenTypes[EQUAL_STATE] = EQUAL_TOKEN;
	mCorrespondingTokenTypes[OR_STATE] = OR_TOKEN;
	mCorrespondingTokenTypes[AND_STATE] = AND_TOKEN;
	mCorrespondingTokenTypes[ASSIGNMENT_STATE] = ASSIGNMENT_TOKEN;
	mCorrespondingTokenTypes[LPAREN_STATE] = LPAREN_TOKEN;
	mCorrespondingTokenTypes[RPAREN_STATE] = RPAREN_TOKEN;
	mCorrespondingTokenTypes[LCURLY_STATE] = LCURLY_TOKEN;
	mCorrespondingTokenTypes[RCURLY_STATE] = RCURLY_TOKEN;
	mCorrespondingTokenTypes[LPAREN_STATE] = LPAREN_TOKEN;
	mCorrespondingTokenTypes[PLUS_STATE] = PLUS_TOKEN;
	mCorrespondingTokenTypes[MINUS_STATE] = MINUS_TOKEN;
	mCorrespondingTokenTypes[TIMES_STATE] = TIMES_TOKEN;
	mCorrespondingTokenTypes[DIVIDE_STATE] = DIVIDE_TOKEN;
	mCorrespondingTokenTypes[NOT_STATE] = NOT_TOKEN;
	mCorrespondingTokenTypes[LINECOMMENT_STATE] = COMMENT_TOKEN;
	mCorrespondingTokenTypes[BLOCKCOM3_STATE] = COMMENT_TOKEN;
	mCorrespondingTokenTypes[SEMICOLON_STATE] = SEMICOLON_TOKEN;
	mCorrespondingTokenTypes[ENDFILE_STATE] = ENDFILE_TOKEN;

}

MachineState StateMachineClass::UpdateState(char currentCharacter, TokenType &correspondingTokenType){
	
	CharacterType charType = BAD_CHAR;
	
	if(isdigit(currentCharacter))
		charType = DIGIT_CHAR;
	else if(isalpha(currentCharacter))
		charType = LETTER_CHAR;
	else if(currentCharacter=='\n')
		charType = NEWLINE_CHAR;
	else if(isspace(currentCharacter))
		charType = WHITESPACE_CHAR;
	else if(currentCharacter=='+')
		charType = PLUS_CHAR;
	else if(currentCharacter=='-')
		charType = MINUS_CHAR;
	else if(currentCharacter=='*')
		charType = TIMES_CHAR;
	else if(currentCharacter=='/')
		charType = DIVIDE_CHAR;
	else if(currentCharacter=='<')
		charType = LESS_CHAR;
	else if(currentCharacter=='>')
		charType = GREATER_CHAR;
	else if(currentCharacter=='=')
		charType = EQUAL_CHAR;
	else if(currentCharacter=='|')
		charType = OR_CHAR;
	else if(currentCharacter=='&')
		charType = AND_CHAR;
	else if(currentCharacter=='!')
		charType = NOT_CHAR;
	else if(currentCharacter=='(')
		charType = LPAREN_CHAR;
	else if(currentCharacter==')')
		charType = RPAREN_CHAR;
	else if(currentCharacter=='{')
		charType = LCURLY_CHAR;
	else if(currentCharacter=='}')
		charType = RCURLY_CHAR;
	else if(currentCharacter==';')
		charType = SEMICOLON_CHAR;
	else if(currentCharacter==EOF)
		charType = ENDFILE_CHAR;
	else {
		charType = BAD_CHAR;
		cerr << "RECEIVED BAD CHARACTER Error: " << currentCharacter << endl;
		exit(1);
	}

	correspondingTokenType = mCorrespondingTokenTypes[mCurrentState];
	mCurrentState = mLegalMoves[mCurrentState][charType];
	return mCurrentState;

}