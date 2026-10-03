#pragma once

namespace Memoria
{
	class CSignature
	{
	private:
		int HexToInt(char ch);

	public:
		std::vector<std::uint8_t> m_pattern;
		std::vector<std::uint8_t> m_mask;
		bool m_hasOptionals;

	public:
		CSignature() = delete;
		CSignature(const std::string_view& str);

		std::vector<std::optional<std::uint8_t>> BuildStdSignature();
	};
}