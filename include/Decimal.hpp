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

#include <iomanip>
#include <limits>
#include <math.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cstdint>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/optional.hpp>

#define PREPROCESSOR_STRINGIZE_A(x) #x
#define PREPROCESSOR_STRINGIZE(x) PREPROCESSOR_STRINGIZE_A(x)
#define STR__LINE__ PREPROCESSOR_STRINGIZE(__LINE__)

namespace tylawin
{
	template<typename T>
	constexpr T constexpr_pow(T x, uint8_t y)
	{
		return y ? x * constexpr_pow(x, y - 1) : 1;
	}

	namespace DataTypes
	{
		class Decimal
		{
		private:
			static const constexpr uint16_t FRACTION_DIGITS = 19;
			static const constexpr uint64_t fractionDenominator_ = constexpr_pow(10, FRACTION_DIGITS);
			static const constexpr uint64_t maxValue_ = fractionDenominator_ - 1;

		public:
			Decimal() : positive_(true), whole_(0), fraction_(0)
			{ }

			Decimal(const Decimal &value) : positive_(value.positive_), whole_(value.whole_), fraction_(value.fraction_)
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

			Decimal(uint64_t value) : positive_(true), whole_(value), fraction_(0)
			{ }

			Decimal(float value)
			{
				positive_ = (value >= 0);
				value = fabsf(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*fractionDenominator_);
			}

			Decimal(double value)
			{
				positive_ = (value >= 0);
				value = fabs(value);
				whole_ = static_cast<uint64_t>(value);
				fraction_ = static_cast<uint64_t>((value - whole_)*fractionDenominator_);
			}

			Decimal(long double value)
			{
				std::stringstream sigDigits;
				sigDigits << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << value;
				*this = Decimal(sigDigits.str());
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
					if(tmp.size() > FRACTION_DIGITS)
						tmp = tmp.substr(0, FRACTION_DIGITS);
					while(tmp.size() < FRACTION_DIGITS)
						tmp += '0';
					fraction_ = std::stoull(tmp);
				}
			}
			Decimal(const char value[])
			{
				*this = Decimal(std::string(value));
			}

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
				fraction_ = static_cast<uint64_t>((value - whole_)*fractionDenominator_);
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
					while(fraction_ > maxValue_)
					{
						whole_ += 1;
						fraction_ -= fractionDenominator_;
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
						fraction_ += 1 * fractionDenominator_;
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

				using namespace boost::multiprecision;

				uint128_t frac = static_cast<uint128_t>(ret.fraction_) * rhs.fraction_;
				frac /= static_cast<uint64_t>(fractionDenominator_);

				uint128_t leftWholeRightFraction = static_cast<uint128_t>(whole_) * rhs.fraction_;
				uint64_t tmp;
				tmp = static_cast<uint64_t>(leftWholeRightFraction / fractionDenominator_);
				ret.whole_ += tmp;
				frac += static_cast<uint64_t>(leftWholeRightFraction - static_cast<uint128_t>(tmp) * fractionDenominator_);
				uint128_t rightWholeLeftFraction = static_cast<uint128_t>(rhs.whole_) * fraction_;
				tmp = static_cast<uint64_t>(rightWholeLeftFraction / fractionDenominator_);
				ret.whole_ += tmp;
				frac += static_cast<uint64_t>(rightWholeLeftFraction - static_cast<uint128_t>(tmp) * fractionDenominator_);

				tmp = static_cast<uint64_t>(frac / fractionDenominator_);
				ret.whole_ += tmp;
				ret.fraction_ = static_cast<uint64_t>(frac - static_cast<uint128_t>(tmp) * fractionDenominator_);
				if(ret.whole_ == 0 && ret.fraction_ == 0)//fix edge case "-0"
					ret.positive_ = true;
				return ret;
			}

			Decimal operator*=(const Decimal &rhs)
			{
				*this = *this * rhs;
				return *this;
			}

			Decimal operator/(const uint64_t rhs) const
			{
				return *this / Decimal(rhs);
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

				using namespace boost::multiprecision;
				uint128_t numerator = static_cast<uint128_t>(whole_) * fractionDenominator_ + fraction_;
				uint128_t denominator = static_cast<uint128_t>(rhs.whole_) * fractionDenominator_ + rhs.fraction_;
				uint128_t quot, rem;
				divide_qr(numerator, denominator, quot, rem);
				ret.whole_ = static_cast<uint64_t>(quot);
				ret.fraction_ = static_cast<uint64_t>(rem * static_cast<uint256_t>(fractionDenominator_) / denominator);//truncate
				return ret;
			}

			Decimal operator/=(uint64_t rhs)
			{
				*this = *this / rhs;
				return *this;
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
					uint8_t m = FRACTION_DIGITS-1;
					rhs.fraction_ = 0;
					while(is.peek() >= '0' && is.peek() <= '9')
					{
						is >> ch;
						rhs.fraction_ += uint8_t(ch - '0') * uint64_t(constexpr_pow(10, m));
						if(m == 0)
							break;
						m--;
					}
				}
				else
					rhs.fraction_ = 0;
				return is;
			}

			long double asDouble() const
			{
				return whole_ + (long double)(fraction_) / fractionDenominator_;
			}

		private:
			void round(int8_t digits)// for 2.2222: -1 would be 2.2  &  1 would be 2
			{
				if(digits < -FRACTION_DIGITS || digits > 20)
					throw std::invalid_argument(__FILE__ ":" STR__LINE__ " - round failed");
				if(digits >= 0)
				{
					fraction_ = 0;
					if(digits > 0)
						whole_ = uint64_t(whole_ / constexpr_pow(10, (uint8_t)digits)) * int64_t(constexpr_pow(10, (uint8_t)digits));
				}
				else
				{
					digits = FRACTION_DIGITS - abs(digits);
					fraction_ = uint64_t(fraction_ / constexpr_pow(10, (uint8_t)digits)) * int64_t(constexpr_pow(10, (uint8_t)digits));
				}
			}

			std::string fractionAsString() const
			{
				if(fraction_ == 0)
					return "0";

				std::string res = std::to_string(fraction_);
				while(res.size() < FRACTION_DIGITS)
					res = "0" + res;
				while(res.size() > 0 && res.back() == '0')
					res.pop_back();
				return res;
			}

			bool positive_;
			uint64_t whole_;
			uint64_t fraction_;
		};

		const constexpr uint16_t Decimal::FRACTION_DIGITS;
		const constexpr uint64_t Decimal::fractionDenominator_;
		const constexpr uint64_t Decimal::maxValue_;

		Decimal operator/(const int64_t lhs, const Decimal &rhs)
		{
			return Decimal(lhs) / rhs;
		}

		std::string to_string(const Decimal &amount, boost::optional<size_t> precision = boost::none)
		{
			std::stringstream tmpss;
			tmpss << amount;
			std::string tmp;
			tmpss >> tmp;

			if(!precision)
				return tmp;

			size_t pos = tmp.find('.');
			if(*precision == 0)
			{
				if(pos == std::string::npos)
					return tmp;
				else
					return tmp.substr(0, pos);
			}
			if(pos == std::string::npos)
			{
				tmp += '.';
				pos = tmp.find('.');
			}
			while(tmp.size() < pos + 1 + *precision)
				tmp += '0';
			if(tmp.size() > pos + 1 + *precision)
				return tmp.substr(0, pos + 1 + *precision);
			else
				return tmp;
		}
	}
}
