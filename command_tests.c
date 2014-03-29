#include "command.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  struct snt_cmd* cmd = snt_cr_cmd(7734, 2, 0, NULL);  
  snt_print_cmd(cmd);
  free(cmd);

  return 0;
}
