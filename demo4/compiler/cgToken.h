#pragma once

enum cgTokenType {
	cgTokenTypeId,
	cgTokenTypeCharacter,
	cgTokenTypeString,
	cgTokenTypeNumber,
	cgTokenTypeTrue,
	cgTokenTypeFalse,
	cgTokenTypeDo,
	cgTokenTypeIf,
	cgTokenTypeFor,
	cgTokenTypeInt,
	cgTokenTypeChar,
	cgTokenTypeElse,
	cgTokenTypeEnum,
	cgTokenTypeGoto,
	cgTokenTypeLong,
	cgTokenTypeVoid,
	cgTokenTypeBreak,
	cgTokenTypeConst,
	cgTokenTypeFalse,
	cgTokenTypeFloat,
	cgTokenTypeShort,
	cgTokenTypeWhile,
	cgTokenTypeDouble,
	cgTokenTypeExtern,
	cgTokenTypeReturn,
	cgTokenTypeStatic,
	cgTokenTypeSizeof,
	cgTokenTypeStruct,
	cgTokenTypeSwitch,
	cgTokenTypeContinue,
	cgTokenTypeVolatile,
	cgTokenTypeDot,
	cgTokenTypeDot2,
	cgTokenTypeDot3,
	cgTokenTypeAssign,
	cgTokenTypeEqual,
	cgTokenTypeNot,
	cgTokenTypeNotEqual,
	cgTokenTypeSum,
	cgTokenTypeSumSum,
	cgTokenTypeSumEqual,
	cgTokenTypeSub,
	cgTokenTypeSubSub,
	cgTokenTypeSubEqual,
	cgTokenTypeMul,
	cgTokenTypeMulEqual,
	cgTokenTypeDiv,
	cgTokenTypeDivEqual,
	cgTokenTypeGreater,
	cgTokenTypeGreaterEqual,
	cgTokenTypeShiftRight,
	cgTokenTypeShiftRightEqual,
	cgTokenTypeLower,
	cgTokenTypeLowerEqual,
	cgTokenTypeShiftLeft,
	cgTokenTypeShiftLeftEqual,
	cgTokenTypeAndBit,
	cgTokenTypeAndLogic,
	cgTokenTypeAndEqual,
	cgTokenTypeOrBit,
	cgTokenTypeOrLogic,
	cgTokenTypeOrEqual,
	cgTokenTypeXor,
	cgTokenTypeXorEqual,
	cgTokenTypeEof
};

class cgToken
{

public:

	unsigned char _type;

	char* _word;

	size_t _length;

	cgToken* _next;

	cgToken(unsigned char type, const char* word, size_t length, cgToken* next);
	
	~cgToken();

	inline unsigned char type();

	inline const char* word();

	inline long length();

	inline bool eof();

};

