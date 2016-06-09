# pshm
php共享内存扩展

```
shm_write(int key, long buf_size, string value);
shm_write(100, 1024, "hello world");
```
```
shm_read(int key);
echo shm_read(100);
```
```
shm_clear(100);
```