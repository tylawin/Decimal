/*
Copyright 2016 Tyler Winters

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <math.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstdint>
#ifdef _WIN32
#include <codecvt>
#endif

#define PREPROCESSOR_STRINGIZE_A(x) #x
#define PREPROCESSOR_STRINGIZE(x) PREPROCESSOR_STRINGIZE_A(x)
#define STR__LINE__ PREPROCESSOR_STRINGIZE(__LINE__)

namespace tylawin
{
	namespace DataTypes
	{
		class Decimal
		{
		public:
			Decimal() : positive_(true), whole_(0), fraction_(0)
			{ }

			Decimal(int32_t value)
			{
				positive_ = (value >= 0);
				value = abs(value);
				whole_ = value;
				fraction_ = 0;
			}

			Decimal(const uint32_t value) : positive_(true), whole_(value), fraction_(0)
			{ }

			Decimal(int64_t value)
			{
				positive_ = (value >= 0);
				value = abs(value);
				whole_ = value;
				fraction_ = 0;
			}

			Decimal(float value)
			{
				positive_ = (value >= 0);
				value = fabsf(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*pow(10, 8));
			}

			Decimal(double value)
			{
				positive_ = (value >= 0);
				value = fabs(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*pow(10, 8));
			}

			Decimal(long double value)
			{
				positive_ = (value >= 0);
				value = fabsl(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*pow(10, 8));
			}

			Decimal(std::string value)
			{
				if(value.find_first_not_of("-01234.56789") != std::string::npos)
					throw std::invalid_argument(__FILE__ ":" STR__LINE__ " - string constructor failed.");
				if(value.size() == 0)
					value = "0";

				if(value[0] == '-')
				{
					positive_ = false;
					value = value.substr(1);
				}
				else
					positive_ = true;

				if(value[0] == '.' && value.size() > 1)
					value = "0" + value;

				size_t periodPos = value.find('.');
				if(periodPos == value.npos)
				{
					whole_ = std::stoull(value);
					fraction_ = 0;
				}
				else
				{
					whole_ = std::stoull(value.substr(0, periodPos));
					std::string tmp = value.substr(periodPos + 1);
					if(tmp.size() > 8)
						tmp = tmp.substr(0, 8);
					while(tmp.size() < 8)
						tmp += '0';
					fraction_ = std::stoull(tmp);
				}
			}
			Decimal(const char value[])
			{
				*this = Decimal(std::string(value));
			}
#ifdef _WIN32
			Decimal(const std::wstring &value)
			{
				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
				*this = Decimal(converter.to_bytes(value));
			}
#endif
			const Decimal& operator=(const Decimal rhs)
			{
				positive_ = rhs.positive_;
				whole_ = rhs.whole_;
				fraction_ = rhs.fraction_;
				return *this;
			}

			const Decimal& operator=(long double value)
			{
				if(value >= 0)
					positive_ = true;
				else
					positive_ = false;
				value = abs(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*pow(10, 8));
				return *this;
			}

			bool operator==(const Decimal &rhs) const
			{
				if(positive_ == rhs.positive_ && whole_ == rhs.whole_ && fraction_ == rhs.fraction_)
					return true;
				else
					return false;
			}

			bool operator!=(const Decimal &rhs) const
			{
				return !(*this == rhs);
			}

			bool operator>(const Decimal &rhs) const
			{
				if(positive_ && !rhs.positive_)
					return true;
				else if(!positive_ && rhs.positive_)
					return false;
				else if(positive_ && rhs.positive_)
				{
					if(whole_ == rhs.whole_)
						return fraction_ > rhs.fraction_;
					return whole_ > rhs.whole_;
				}
				else// if(!positive_ && !rhs.positive_)
				{
					if(whole_ == rhs.whole_)
						return fraction_ < rhs.fraction_;
					return whole_ < rhs.whole_;
				}
			}

			bool operator>=(const Decimal &rhs) const
			{
				if(*this == rhs)
					return true;
				return *this > rhs;
			}

			bool operator<(const Decimal &rhs) const
			{
				return !(*this >= rhs);
			}

			bool operator<=(const Decimal &rhs) const
			{
				if(*this == rhs)
					return true;
				return *this < rhs;
			}

			Decimal operator+(const Decimal &rhs) const
			{
				Decimal tmp(*this);
				tmp += rhs;
				return tmp;
			}

			Decimal operator+=(const Decimal &rhs)
			{
				if(positive_ && rhs.positive_)
				{
					whole_ += rhs.whole_;
					fraction_ += rhs.fraction_;
					while(fraction_ > 99999999)
					{
						whole_ += 1;
						fraction_ -= 100000000;
					}
				}
				else if(positive_ && !rhs.positive_)
				{
					*this = *this - (-rhs);
				}
				else if(!positive_ && rhs.positive_)
				{
					*this = rhs - (-*this);
				}
				else// if(! && !)
				{
					*this = -((-*this) + (-rhs));
				}
				return *this;
			}

			Decimal operator-() const
			{
				Decimal tmp(*this);
				if(whole_ == 0 && fraction_ == 0)
					tmp.positive_ = true;
				else
					tmp.positive_ = !positive_;
				return tmp;
			}

			Decimal operator-(const Decimal &rhs) const
			{
				Decimal tmp(*this);
				tmp -= rhs;
				return tmp;
			}

			Decimal operator-=(const Decimal &rhs)
			{
				if(!rhs.positive_)
					*this = *this + -(rhs);
				else if(!positive_ && rhs.positive_ || whole_ < rhs.whole_ || (whole_ == rhs.whole_ && fraction_ < rhs.fraction_))
				{
					*this = -(rhs - *this);
				}
				else// if(positive_ && !rhs.positive_ || whole_ >= rhs.whole_)
				{
					whole_ -= rhs.whole_;
					if(fraction_ >= rhs.fraction_)
						fraction_ -= rhs.fraction_;
					else
					{
						whole_ -= 1;
						fraction_ += 1 * uint64_t(pow(10, 8));
						fraction_ -= rhs.fraction_;
					}
				}
				return *this;
			}

			Decimal operator*(const Decimal &rhs) const
			{
				Decimal ret(*this);
				if(positive_ && rhs.positive_ || !positive_ && !rhs.positive_)
					ret.positive_ = true;
				else
					ret.positive_ = false;
				ret.whole_ *= rhs.whole_;
				ret.fraction_ *= rhs.fraction_;
				ret.fraction_ /= uint64_t(pow(10, 8));
				uint64_t leftWholeRightFraction = whole_ * rhs.fraction_;
				uint64_t tmp;
				tmp = leftWholeRightFraction / pow(10, 8);
				ret.whole_ += tmp;
				ret.fraction_ += leftWholeRightFraction - tmp * pow(10, 8);
				uint64_t rightWholeLeftFraction = rhs.whole_ * fraction_;
				tmp = rightWholeLeftFraction / pow(10, 8);
				ret.whole_ += tmp;
				ret.fraction_ += rightWholeLeftFraction - tmp * pow(10, 8);

				tmp = ret.fraction_ / pow(10, 8);
				ret.whole_ += tmp;
				ret.fraction_ -= tmp * pow(10, 8);
				if(ret.whole_ == 0 && ret.fraction_ == 0)//fix edge case "-0"
					ret.positive_ = true;
				return ret;
			}

			Decimal operator*=(const Decimal &rhs)
			{
				*this = *this * rhs;
				return *this;
			}

			Decimal operator/(const Decimal &rhs) const
			{
				Decimal ret(*this);
				if(rhs.whole_ == 0 && rhs.fraction_ == 0)
					throw std::runtime_error(__FILE__ ":" STR__LINE__ " - division by zero");
				if(positive_ && rhs.positive_ || !positive_ && !rhs.positive_)
					ret.positive_ = true;
				else
					ret.positive_ = false;
				//if(rhs.whole_ > pow(10, 8))
				//	throw std::runtime_error(__FILE__ ":" STR__LINE__ " - underflow");
				int64_t numerator = whole_ * uint64_t(pow(10, 8)) + fraction_;
				int64_t denominator = rhs.whole_ * uint64_t(pow(10, 8)) + rhs.fraction_;
				auto res = std::div(numerator, denominator);
				ret.whole_ = res.quot;
				ret.fraction_ = res.rem * int64_t(pow(10, 8)) / denominator;//truncate
				//rounding at half 1.0000'0000'5
				//	res = std::div(res.rem * int64_t(pow(10, 8)), denominator);
				//	ret.fraction_ = res.quot + (res.rem * 2 > denominator ? 1 : 0);
				return ret;
			}

			Decimal operator/=(const Decimal &rhs)
			{
				*this = *this / rhs;
				return *this;
			}

			friend std::ostream& operator<<(std::ostream& os, const Decimal &rhs)
			{
				if(!rhs.positive_)
					os << '-';
				os << rhs.whole_;
				if(rhs.fraction_ > 0)
					os << '.' << rhs.fractionAsString();
				return os;
			}
			friend std::istream& operator >> (std::istream& is, Decimal &rhs)
			{
				char ch;
				if(is.peek() == '-')
				{
					rhs.positive_ = false;
					is >> ch;
				}
				else
					rhs.positive_ = true;

				bool missingWhole = false;
				if(is.peek() == '.')
				{
					rhs.whole_ = 0;
					missingWhole = true;
				}
				else
					is >> rhs.whole_;

				if(is.peek() == '.')
				{
					is >> ch;
					if(missingWhole && !(is.peek() >= '0' && is.peek() <= '9'))
						throw std::invalid_argument(__FILE__ ":" STR__LINE__ " - operator>> failed");
					size_t m = 7;
					rhs.fraction_ = 0;
					while(is.peek() >= '0' && is.peek() <= '9')
					{
						is >> ch;
						rhs.fraction_ += uint8_t(ch - '0') * uint64_t(pow(10, m));
						if(m == 0)
							break;
						m--;
					}
				}
				else
					rhs.fraction_ = 0;
				return is;
			}
#ifdef _WIN32
			friend std::wostream& operator<<(std::wostream& os, const Decimal &rhs)
			{
				if(!rhs.positive_)
					os << L'-';
				os << rhs.whole_;
				if(rhs.fraction_ > 0)
				{
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
					os << L'.' << converter.from_bytes(rhs.fractionAsString());
				}
				return os;
			}

			friend std::wistream& operator >> (std::wistream& is, Decimal &rhs)
			{
				wchar_t ch;
				if(is.peek() == L'-')
				{
					rhs.positive_ = false;
					is >> ch;
				}
				else
					rhs.positive_ = true;

				bool missingWhole = false;
				if(is.peek() == '.')
				{
					rhs.whole_ = 0;
					missingWhole = true;
				}
				else
					is >> rhs.whole_;

				if(is.peek() == L'.')
				{
					is >> ch;
					if(missingWhole && !(is.peek() >= L'0' && is.peek() <= L'9'))
						throw std::invalid_argument(__FILE__ ":" STR__LINE__ " - operator>> failed");
					size_t m = 7;
					rhs.fraction_ = 0;
					while(is.peek() >= L'0' && is.peek() <= L'9')
					{
						is >> ch;
						rhs.fraction_ += uint32_t(ch - L'0') * uint64_t(pow(10, m));
						if(m == 0)
							break;
						m--;
					}
				}
				else
					rhs.fraction_ = 0;
				return is;
			}
#endif
			long double asDouble() const
			{
				return whole_ + (long double)(fraction_) / 100000000llu;
			}

			std::string asString() const //TODO: make private, swap implementation with operator>>, use to_string outside class
			{
				std::stringstream ss;
				ss << *this;
				std::string tmp;
				ss >> tmp;
				return tmp;
			}

		private:
			void round(int digits)// for 2.2222: -1 would be 2.2  &  1 would be 2
			{
				if(digits < -8 || digits > 20)
					throw std::invalid_argument(__FILE__ ":" STR__LINE__ " - round failed");
				if(digits >= 0)
				{
					fraction_ = 0;
					if(digits > 0)
						whole_ = uint64_t(whole_ / pow(10, digits)) * int64_t(pow(10, digits));
				}
				else
				{
					digits = 8 - abs(digits);
					fraction_ = uint64_t(fraction_ / pow(10, digits)) * int64_t(pow(10, digits));
				}
			}

			std::string fractionAsString() const
			{
				if(fraction_ == 0)
					return "0";

				std::string res = std::to_string(fraction_);
				while(res.size() < 8)
					res = "0" + res;
				while(res.size() > 0 && res.back() == '0')
					res.pop_back();
				return res;
			}

			bool positive_;
			uint64_t whole_;
			uint64_t fraction_;//8 places
		};

		Decimal operator/(int64_t lhs, const Decimal &rhs)
		{
			Decimal ret = Decimal(lhs) / rhs;
			return ret;
		}

		std::string to_string(const Decimal &amount, size_t precision = 8)//TODO: if no precision default format (remove '0' at end...)
		{
			std::stringstream tmpss;
			tmpss << amount;
			std::string tmp;
			tmpss >> tmp;
			size_t pos = tmp.find('.');
			if(pos == std::string::npos)
				return tmp;
			else if(precision == 0)
				return tmp.substr(0, pos);
			else
				return tmp.substr(0, pos + 1 + precision);
		}
	}
}
