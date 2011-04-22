#include "tuplemerger.h"
#include "logstore.h"

// t2 is the newer tuple.
// we return deletes here.  our caller decides what to do with them.
datatuple* tuplemerger::merge(const datatuple *t1, const datatuple *t2)
{
    datatuple *t;
    if(t1->isDelete() && t2->isDelete()) {
		t = t2->create_copy();
    } else if(t1->isDelete()) //delete -> t2
    {
        t = t2->create_copy();
    }
    else if(t2->isDelete())
    {
        t = t2->create_copy();
    }
    else //neither is a delete
    {
        t = (*merge_fp)(t1,t2);
    }
    
    return t;
}
/**
 * appends the data in t2 to data from t1
 * 
 * deletes are handled by the tuplemerger::merge function
 * so here neither t1 nor t2 is a delete datatuple
 **/
datatuple* append_merger(const datatuple *t1, const datatuple *t2)
{
	assert(!(t1->isDelete() || t2->isDelete()));
    len_t keylen = t1->keylen();
    len_t datalen = t1->datalen() + t2->datalen();
    byte * data = (byte*)malloc(datalen);
    memcpy(data, t1->data(), t1->datalen());
    memcpy(data + t1->datalen(), t2->data(), t2->datalen());

	return datatuple::create(t1->key(), keylen, data, datalen);
}

/**
 * replaces the data with data from t2
 * 
 * deletes are handled by the tuplemerger::merge function
 * so here neither t1 nor t2 is a delete datatuple
 **/
datatuple* replace_merger(const datatuple *t1, const datatuple *t2)
{
	return t2->create_copy();
}
