#include "defines.h"
#include "queue_array.h"
#include "queue_list.h"
#include "process.h"
#include "modeling.h"
#include "print.h"

int main(void)
{
    int rc = 0;

    // free_addr_t free_addr;
    // free_addr.free_addrs = NULL;
    // free_addr.size = MAX_FREE_ADDR_SIZE;

    // if ((rc = create_free_addr(&free_addr)) != 0)
    //     return rc;

    int work;

    if ((rc = read_work_num(&work)) != 0)
        return rc;

    if (!work)
    {
        if ((rc = do_modeling()) != 0)
            return rc;
    }
    else
    {
        if ((rc = do_action()) != 0)
            return rc;
    }

    // free(free_addr.free_addrs);

    return rc;
}
