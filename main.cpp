#define value_type double
#include <fcsr.h>

int main(int argc, char **argv) {
    if (csr*mtx = csrLoad(argv[1])) {
        for (int i = 0; i < mtx->m; ++i) printf("%d%c", mtx->row[i], i + 1 < mtx->m? '\t': '\n');
        for (int i = 0; i < mtx->nnz; ++i) printf("%d%c", mtx->col[i], i + 1 < mtx->nnz? '\t': '\n');
        if (mtx->val) for (int i = 0; i < mtx->nnz; ++i) printf("%g%c", mtx->val[i], i + 1 < mtx->nnz? '\t': '\n');
        if (mtx->val_im) for (int i = 0; i < mtx->nnz; ++i) printf("%g%c", mtx->val_im[i], i + 1 < mtx->nnz? '\t': '\n');
        free_csr(mtx);
    }
    return 0;
}
