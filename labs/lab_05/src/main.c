#include "defines.h"
#include "queue_array.h"
#include "gen_number.h"
#include "queue_list.h"
#include "process.h"

int main(void)
{
    int rc = 0;

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

    return rc;
}
