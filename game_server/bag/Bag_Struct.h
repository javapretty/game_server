/*
 * Bag_Struct.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef PACKER_STRUCT_H_
#define PACKER_STRUCT_H_

#include "Bag_Def.h"
#include "Public_Struct.h"

struct Id_Amount {
	Id_Amount(uint32_t id_= 0, int32_t amount_ = 0, Bind_Verify bind_verify_ = BIND_NONE) :
			id(id_), amount(amount_), bind_verify(bind_verify_) {
	}
	uint32_t id;
	int32_t amount;
	Bind_Verify bind_verify;
	friend bool operator<(const Id_Amount &id_amount1, const Id_Amount &id_amount2) {
		if (id_amount1.id < id_amount2.id) return true;
		if (id_amount1.id == id_amount2.id && id_amount1.bind_verify > id_amount2.bind_verify) return true;
		return false;
	}

	Id_Amount& operator= (const Id_Amount& id_amount) {
		this->id = id_amount.id;
		this->amount = id_amount.amount;
		return *this;
	}
};

struct Index_Amount {
	Index_Amount(uint32_t index_ = 0, int32_t amount_ = 0, Bind_Verify bind_verify_  = BIND_NONE) :
			index(index_), amount(amount_), bind_verify(bind_verify_) {
	}
	uint32_t index;
	int32_t amount;
	Bind_Verify bind_verify;

	int serialize(Block_Buffer &buf) const{
		buf.write_uint32(index);
		buf.write_int32(amount);
		buf.write_uint8(bind_verify);
		return 0;
	}
	int deserialize(Block_Buffer &buf) {
		buf.read_uint32(index);
		buf.read_int32(amount);
		uint8_t bind_verify_ = 0;
		buf.read_uint8(bind_verify_);
		bind_verify = static_cast<Bind_Verify>(bind_verify_);
		return 0;
	}

	friend bool operator<(const Index_Amount &index_amount1, const Index_Amount &index_amount2) {
		if (index_amount1.index < index_amount2.index) return true;
		if (index_amount1.index == index_amount2.index && index_amount1.bind_verify > index_amount2.bind_verify) return true;
		return false;
	}
};

// 若要使用引用减少一些值拷贝，可以参考这里。不过会让代码变得古怪
// http://stackoverflow.com/questions/634662/non-static-const-member-cant-use-default-assignment-operator
struct Money_Add_Info {
	explicit Money_Add_Info(Money_Type type_, int nums_) : type(type_), nums(nums_) {}

	Money_Type type;
	int nums;
};

struct Money_Sub_Info {
	explicit Money_Sub_Info(Money_Sub_Type type_, int nums_) : type(type_), nums(nums_) {}

	Money_Sub_Type type;
	int nums;
};


#endif /* PACKER_STRUCT_H_ */
