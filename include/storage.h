#ifndef STORAGE_H
#define STORAGE_H

typedef struct Storage Storage;

// 创建和释放存储实例
Storage* storage_create();
void storage_free(Storage* storage);

// 设置键值对
int storage_set(Storage* storage, const char* key, const char* value);

// 获取键对应的值
const char* storage_get(Storage* storage, const char* key);

#endif
