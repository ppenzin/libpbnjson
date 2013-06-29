// @@@LICENSE
//
//      Copyright (c) 2009-2013 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// LICENSE@@@

#ifndef JOBJECT_INTERNAL_H_
#define JOBJECT_INTERNAL_H_

#include <japi.h>
#include <jtypes.h>
#include <glib.h>

#define ARRAY_BUCKET_SIZE (1 << 4)
#define OUTSIDE_ARR_BUCKET_RANGE(value) ((value) & (~(ARRAY_BUCKET_SIZE - 1)))


typedef enum {
	JV_NULL = 0,
	JV_BOOL,
	JV_NUM,
	JV_STR,
	JV_ARRAY,
	JV_OBJECT,
} JValueType;

typedef struct PJSON_LOCAL {
	bool value;
} jbool;

typedef enum {
	NUM_RAW,
	NUM_FLOAT,
	NUM_INT,
} JNumType;

typedef struct PJSON_LOCAL {
	union {
		raw_buffer raw;
		double floating;
		int64_t integer;
	} value;
	JNumType m_type;
	ConversionResultFlags m_error;
	jdeallocator m_rawDealloc;
} jnum;

typedef struct PJSON_LOCAL {
	jdeallocator m_dealloc;
	raw_buffer m_data;
} jstring;

typedef struct PJSON_LOCAL {
	jvalue_ref m_smallBucket[ARRAY_BUCKET_SIZE];
	jvalue_ref *m_bigBucket;
	ssize_t m_size;
	ssize_t m_capacity;
} jarray;

typedef struct PJSON_LOCAL {
	GHashTable *m_members;
} jobject;

struct jvalue {
	union {
		jbool val_bool;
		jnum val_num;
		jstring val_str;
		jarray val_array;
		jobject val_obj;
	} value;
	JValueType m_type;
	ssize_t m_refCnt;
	char *m_toString;
	jdeallocator m_toStringDealloc;
	raw_buffer m_backingBuffer;
	bool m_backingBufferMMap;
};

typedef struct PJSON_LOCAL jvalue jvalue;

extern PJSON_LOCAL jvalue JNULL;

PJSON_LOCAL bool jobject_init(jobject *obj);

extern PJSON_LOCAL int64_t jnumber_deref_i64(jvalue_ref num);

extern PJSON_LOCAL bool jboolean_deref(jvalue_ref boolean);

extern PJSON_LOCAL bool jbuffer_equal(raw_buffer buffer1, raw_buffer buffer2);

extern PJSON_LOCAL raw_buffer jnumber_deref_raw(jvalue_ref num);

#endif /* JOBJECT_INTERNAL_H_ */
