//
//  TypedStream.cpp
//  Developed by Jian Qin & Xiaoqiang Qi
//  Mentored by Rick Gessner
//

#include "TypedStream.hpp"

namespace SQLightning {
	StatusResult StreamEncoder::setStream(std::ostream* aStream) {
		if (!aStream) {
			return StatusResult{ streamExpected };
		}
		theStream = aStream;
		return StatusResult{};
	}

	StreamEncoder& StreamEncoder::encode(Value aValue) {
		switch (aValue.index())
		{
		case 0:
			*theStream << "b " << std::get<bool>(aValue) << " ";
			break;
		case 1:
			*theStream << "i " << std::get<int>(aValue) << " ";
			break;
		case 2:
			*theStream << "d " << std::get<double>(aValue) << " ";
			break;
		case 3:
			*theStream << "s \"" << std::get<std::string>(aValue) << "\" ";
			break;
		default:
			break;
		}
		return *this;
	}


	StatusResult StreamDecoder::setStream(std::istream* aStream) {
		if (!aStream) {
			return StatusResult{ streamExpected };
		}
		theStream = aStream;
		return StatusResult{};
	}

	StreamDecoder& StreamDecoder::decode(int &aNumber) {
		if ("i" == theTokenizer.current().data) {
			theTokenizer.next();
			aNumber = stoi(theTokenizer.current().data);
			theTokenizer.next();
		}
		return *this;
	}

	StreamDecoder& StreamDecoder::decode(size_t& anUnsignedNumber) {
		if ("i" == theTokenizer.current().data) {
			theTokenizer.next();
			anUnsignedNumber = stoi(theTokenizer.current().data);
			theTokenizer.next();
		}
		return *this;
	}

	StreamDecoder& StreamDecoder::decode(bool& aBoolean) {
		if ("b" == theTokenizer.current().data) {
			theTokenizer.next();
			aBoolean = "1" == theTokenizer.current().data ? true : false;
			theTokenizer.next();
		}
		return *this;
	}
	StreamDecoder& StreamDecoder::decode(double& aDouble) {
		if ("d" == theTokenizer.current().data) {
			theTokenizer.next();
			aDouble = stod(theTokenizer.current().data);
			theTokenizer.next();
		}
		return *this;
	}
	StreamDecoder& StreamDecoder::decode(std::string& aStr) {
		if ("s" == theTokenizer.current().data) {
			theTokenizer.next();
			aStr = theTokenizer.current().data;
			theTokenizer.next();
		}
		return *this;
	}


	StreamDecoder& StreamDecoder::decode(Value& aValue) {
		switch (theTokenizer.current().data[0])
		{
		case 'i': {
			int anInt;
			decode(anInt);
			aValue = anInt;
			break;
		}
		case 'b': {
			bool aBoolean;
			decode(aBoolean);
			aValue = aBoolean;
			break;
		}
		case 'd': {
			double d;
			decode(d);
			aValue = d;
			break;
		}
		case 's': {
			std::string aString;
			decode(aString);
			aValue = aString;
			break;
		}
		default:
			break;
		}
		return *this;
	}

}
