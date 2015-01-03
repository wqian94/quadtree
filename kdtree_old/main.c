#include <stdio.h>
#include <stdlib.h>

#include "./KDtree.h"

int main(int argc, char* argv[]) {
    setbuf(stdout, 0);
    KDtree* qt = KDtree_create(1024, (Point){0, 0});
    printf("%lf %lf %f\n", qt->center.val[0], qt->center.val[1], qt->length);
    return 0;
}
