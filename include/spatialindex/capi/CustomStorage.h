/******************************************************************************
 * Project:  libsidx - A C API wrapper around libspatialindex
 * Purpose:  C++ object declarations to implement the custom storage manager.
 * Author:   Matthias (nitro), nitro@dr-code.org
 ******************************************************************************
 * Copyright (c) 2010, Matthias (nitro)
 *
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
******************************************************************************/

#pragma once

#include "sidx_export.h"

namespace SpatialIndex
{
	namespace StorageManager
	{
        struct SIDX_DLL CustomStorageManagerCallbacks
        {
            CustomStorageManagerCallbacks() 
            = default;

            void* context{nullptr};
            void (*createCallback)( const void* context, int* errorCode ){nullptr};
            void (*destroyCallback)( const void* context, int* errorCode ){nullptr};
			void (*flushCallback)( const void* context, int* errorCode );
            void (*loadByteArrayCallback)( const void* context, const id_type page, uint32_t* len, byte** data, int* errorCode ){nullptr};
            void (*storeByteArrayCallback)( const void* context, id_type* page, const uint32_t len, const byte* const data, int* errorCode ){nullptr};
            void (*deleteByteArrayCallback)( const void* context, const id_type page, int* errorCode ){nullptr};
        };

        class SIDX_DLL CustomStorageManager : public SpatialIndex::IStorageManager
        {
        public:
            // I'd like this to be an enum, but casting between enums and ints is not nice
            static const int NoError = 0;
            static const int InvalidPageError = 1;
            static const int IllegalStateError = 2;

	        CustomStorageManager(Tools::PropertySet&);

	        ~CustomStorageManager() override;

			void flush() override;
	        void loadByteArray(const id_type page, uint32_t& len, byte** data) override;
	        void storeByteArray(id_type& page, const uint32_t len, const byte* const data) override;
	        void deleteByteArray(const id_type page) override;

        private:
            CustomStorageManagerCallbacks   callbacks;

            inline void processErrorCode(int errorCode, const id_type page);
        }; // CustomStorageManager

        // factory function
        IStorageManager* returnCustomStorageManager(Tools::PropertySet& in);
    }
}

