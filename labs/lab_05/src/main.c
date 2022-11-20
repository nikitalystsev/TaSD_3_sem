#include "defines.h"
#include "queue_array.h"
#include "gen_number.h"
#include "queue_list.h"
#include "process.h"

int main(void)
{
    int rc = 0;

    parametres_t param;
    fill_default_parametres(&param);

    rc = modeling_process_list(&param);

    return rc;
}
