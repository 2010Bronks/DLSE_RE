#pragma once

namespace Memoria
{
	class CMemoryPatch
	{
	private:
		void* _dest_address;

		std::vector<uint8_t> _data_origin;
		std::vector<uint8_t> _data_patch;

		std::vector<void*> _stack_backtrace;
		bool _modify_protect;
		bool _active;

		void Toggle(bool state);

	public:
		CMemoryPatch(void* dest_address, const void* source_address, size_t size, bool modify_prot);
		~CMemoryPatch();

		bool IsActive() const;
		bool IsValid() const;

		void Apply();
		void Restore();

		void* GetAddress() const { return _dest_address; }

		const auto& GetDataOrigin() const { return _data_origin; }
		const auto& GetDataPatch() const { return _data_patch; }
	};
}