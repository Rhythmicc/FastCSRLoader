#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef value_type
#define value_type float
#endif // !value_type

typedef struct {
    unsigned int m, n, nnz;
    unsigned int *row, *col;
    value_type* val;
} csr;

// allocate a csr data structure
csr*__new_csr(unsigned int m, unsigned int n, unsigned int nnz) {
    csr* res = (csr*) malloc(sizeof(csr));
    res->m = m;
    res->n = n;
    res->nnz = nnz;

    res->row = (unsigned int*) malloc(sizeof(unsigned int) * (m + 1));
    res->col = (unsigned int*) malloc(sizeof(unsigned int) * nnz);
    res->val = (value_type*) malloc(sizeof(value_type) * nnz);
    return res;
}

csr* csrLoad(const char*filePath) {
    csr*res = 0;
    
    int fd = open(filePath, O_RDONLY), m, n, nnz;
    if (fd < 0) return res;
    size_t len = lseek(fd, 0, SEEK_END) ;

    char*mbuf = (char*) mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    char*data = (char*) malloc(len);
    memcpy(data, mbuf, len);
    close(fd);
    munmap(mbuf, len);

    char*line = strtok(data, "\n");
    while (line != NULL && line[0] == '%') line = strtok(NULL, "\n");
    sscanf(line, "%d%d%d", &m, &n, &nnz);
    
    unsigned int* cnt_row = (unsigned int*) calloc(m + 1, sizeof(unsigned int));
    unsigned int* ia = (unsigned int*) malloc(sizeof(unsigned int) * nnz);
    unsigned int* ja = (unsigned int*) malloc(sizeof(unsigned int) * nnz);
    value_type* val = (value_type*) malloc(sizeof(value_type) * nnz);

    line = strtok(NULL, "\n");

    for(int index = 0; index < nnz; ++index) {
        sscanf(line, sizeof(value_type) == sizeof(float)?"%d%d%f": "%d%d%lf", ia + index, ja + index, val + index);
        ++cnt_row[ia[index] + 1];        
        if((line = strtok(NULL, "\n")) == NULL && (index + 1 < nnz)) {
            free(data), free(ia), free(ja), free(val), free(cnt_row);
            return res;
        }
    }
    
    for (int i = 0; i < m; ++i) cnt_row[i+1] += cnt_row[i];
    
    res = __new_csr(m, n, nnz);
    memcpy(res->row, cnt_row, sizeof(unsigned int) * (m + 1));
    memset(cnt_row, 0, sizeof(unsigned int) * (m + 1));

    for (int index = 0; index < nnz; ++index) {
        unsigned int offset = res->row[ia[index]] + cnt_row[ia[index]];
        res->col[offset] = ja[index];
        res->val[offset] = val[index];
        ++cnt_row[ia[index]];
    }

    free(data), free(ia), free(ja), free(val), free(cnt_row);
    return res;
}

void free_csr(csr*csr_p) {
    free(csr_p->row);
    free(csr_p->col);
    free(csr_p->val);
    free(csr_p);
}

#ifdef __cplusplus
}
#endif