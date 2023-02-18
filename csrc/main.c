#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static napi_value Add(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_number || valuetype1 != napi_number) {
    napi_throw_type_error(env, NULL, "Wrong arguments");
    return NULL;
  }

  double value0;
  status = napi_get_value_double(env, args[0], &value0);
  assert(status == napi_ok);

  double value1;
  status = napi_get_value_double(env, args[1], &value1);
  assert(status == napi_ok);

  napi_value sum;
  status = napi_create_double(env, value0 + value1, &sum);
  assert(status == napi_ok);

  return sum;
}
float** cache;
//init cache
void init_cache(int size){
    cache = (float**)malloc(sizeof(float*)*size);
    for (int i = 0; i < size; i++)
    {
        cache[i] = NULL;
    }
}

void array_cache(napi_env env,napi_value v, int id){

    napi_status status;
    napi_valuetype valuetype;
    status = napi_typeof(env, v, &valuetype);

    uint32_t length;
    status = napi_get_array_length(env, v, &length);

    cache[id] = (float*)malloc(sizeof(float)*length);



    for (unsigned int i = 0; i < length; i++)
    {
        napi_value e;
        status = napi_get_element(env, v, i, &e);
   
        napi_valuetype valuetype;
        status = napi_typeof(env, e, &valuetype);
   
    
        double value;
         
        status = napi_get_value_double(env, e, &value);
     
        cache[id][i] = (float)value;
    }    
}


napi_value New(napi_env env, napi_callback_info info)
{
  size_t argc = 2;
  napi_value args[2];
  napi_status status;

  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);  
  napi_valuetype valuetype0;
  status = napi_typeof(env, args[1], &valuetype0);  
  napi_value ret;
  status = napi_create_array(env, &ret);
  uint32_t i, length;
  status = napi_get_array_length(env, args[1], &length);

  for (i = 0; i < length; i++)
  {
    napi_value e;
   
    status = napi_get_element(env, args[1], i, &e);

   status = napi_set_element(env, ret, i, e);
  }
  int id;
  napi_get_value_int32(env, args[0], &id);

  array_cache(env, ret, id);
  return ret;
}

napi_value getArray(napi_env env, napi_callback_info info){
   size_t argc = 2;
  napi_value args[2];

    napi_value ret;
    napi_status status;
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);  
    status = napi_create_array(env, &ret);
    int id;
    napi_get_value_int32(env, args[0], &id);
    int length;
    napi_get_value_int32(env, args[1], &length);

    for ( int i = 0; i < length; i++)
    {
        napi_value e;
        status = napi_create_double(env, cache[id][i], &e);

        status = napi_set_element(env, ret, i, e);
    }
    return ret;

}

napi_value addVector(napi_env env, napi_callback_info info){
      int id1, id2, length, targetId;
    size_t argc = 4;
    napi_value args[4];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &id2);
    napi_get_value_int32(env, args[2], &length);
    nan_get_value_int32(env, args[3], &targetId);
    omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
        cache[targetId][i] =cache[id1][i]+cache[id2][i];
    }
   
    return NULL;
}

napi_value dotProd(napi_env env, napi_callback_info info){
      int id1, id2, length;
    size_t argc = 3;
    napi_value args[3];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &id2);
    napi_get_value_int32(env, args[2], &length);
    

    float sum = 0;
        omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
       sum+= cache[id1][i] * cache[id2][i];
    }
    //convert to napi_value
    napi_value ret;
    status = napi_create_double(env, sum, &ret);
    return ret;
}
//calculate the absolute value of a vector
napi_value absVector(napi_env env, napi_callback_info info){
      int id1, length;
    size_t argc = 2;
    napi_value args[2];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &length);
    int* tmp;
    tmp = (int*)malloc(length * sizeof(int));
    omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
        tmp[i] = cache[id1][i] * cache[id1][i];
    }
    float sum = 0;
    for(int i = 0; i < length; i++){
        sum += tmp[i];
    }
    sum = sqrt(sum);
    //convert to napi_value
    napi_value ret;
}
//multiply a vector by a scalar
napi_value multVector(napi_env env, napi_callback_info info){
      int id1, length;
    size_t argc = 3;
    napi_value args[3];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &length);
    float scalar;
    napi_get_value_double(env, args[2], &scalar);
    omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
        cache[id1][i] *= scalar;
    }
    return NULL;
}
//divide a vector by a scalar
napi_value divVector(napi_env env, napi_callback_info info){
      int id1, length;
    size_t argc = 3;
    napi_value args[3];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &length);
    float scalar;
    napi_get_value_double(env, args[2], &scalar);
    omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
        cache[id1][i] /= scalar;
    }
    return NULL;
}
//add a scalar to a vector
napi_value addScalar(napi_env env, napi_callback_info info){
      int id1, length;
    size_t argc = 3;
    napi_value args[3];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_value_int32(env, args[0], &id1);
    napi_get_value_int32(env, args[1], &length);
    float scalar;
    napi_get_value_double(env, args[2], &scalar);
    omp_set_num_threads(8);
    #pragma omp parallel for simd
    for(int i = 0; i < length; i++){
        cache[id1][i] += scalar;
    }
    return NULL;
}










#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
    init_cache(10);
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("add", Add);
  status = napi_define_properties(env, exports, 1, &addDescriptor);

    napi_property_descriptor newDescriptor = DECLARE_NAPI_METHOD("newVector", New);
    status = napi_define_properties(env, exports, 1, &newDescriptor);

    napi_property_descriptor getArrayDescriptor = DECLARE_NAPI_METHOD("getVector", getArray);
    status = napi_define_properties(env, exports, 1, &getArrayDescriptor);
    napi_property_descriptor addVectorDescriptor = DECLARE_NAPI_METHOD("addVector", addVector);
    status = napi_define_properties(env, exports, 1, &addVectorDescriptor);
     napi_property_descriptor dotProductDescriptor = DECLARE_NAPI_METHOD("dotProd", dotProd);
    status = napi_define_properties(env, exports, 1, &dotProductDescriptor);  
    napi_property_descriptor absVectorDescriptor = DECLARE_NAPI_METHOD("absVector", absVector);
    status = napi_define_properties(env, exports, 1, &absVectorDescriptor);
    napi_property_descriptor multVectorDescriptor = DECLARE_NAPI_METHOD("multVector", multVector);
    status = napi_define_properties(env, exports, 1, &multVectorDescriptor);
    napi_property_descriptor divVectorDescriptor = DECLARE_NAPI_METHOD("divVector", divVector);
    status = napi_define_properties(env, exports, 1, &divVectorDescriptor);
    napi_property_descriptor addScalarDescriptor = DECLARE_NAPI_METHOD("addScalar", addScalar);
    status = napi_define_properties(env, exports, 1, &addScalarDescriptor);

  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)