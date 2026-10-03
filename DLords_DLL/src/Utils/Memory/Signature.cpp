#include "precompiled.hpp"

Memoria::CSignature::CSignature(const std::string_view& str)
	: m_pattern{}, m_mask{}, m_hasOptionals(false)
{
	size_t i = 0;

	while (i < str.size())
	{
		while (i < str.size() && str[i] == ' ')
			i++;

		if (i >= str.size())
		{
			return;
		}
		else if (str[i] == '?')
		{
			m_hasOptionals = true;

			m_pattern.push_back('\x00');
			m_mask.push_back('?');

			while (i < str.size() && str[i] == '?')
				i++;
		}
		else
		{
			auto nibble_l = HexToInt(str[i]); i++;
			auto nibble_r = HexToInt(str[i]); i++;

			m_pattern.push_back((nibble_l << 4) | (nibble_r));
			m_mask.push_back('x');
		}
	}
}

std::vector<std::optional<uint8_t>> Memoria::CSignature::BuildStdSignature()
{
	std::vector<std::optional<uint8_t>> std_sig;
	std_sig.reserve(m_pattern.size());

	for (size_t i = 0; i < m_pattern.size(); i++)
	{
		if (m_mask[i] == 'x')
			std_sig.push_back(m_pattern[i]);
		else
			std_sig.push_back(std::nullopt);
	}

	return std_sig;
}

int Memoria::CSignature::HexToInt(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;

	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;

	return 0;
}