
#include "l_extern.h"

#define DATAFIELD       0x1F
#define BACKGROUND      0x1E

#define DDCTL1FILE      "c:\\banker\\forms\\ddctl1.dat"
#define DDCTL2FILE      "c:\\banker\\forms\\ddctl2.dat"

#define RECORDSIZE      48L     /* size of a BB data dictionary record */

/*****************************************************************************
**                                                                          **
**  BBDD:       convert the BANCStar prompt file to a BRANCHBANKER format   **
**              data dictionary by inserting the names and types of all of  **
**              the prompts into an existing BB data dictionary file.       **
**                                                                          **
**  input:      BB data dictionary files DDCTL1.DAT and DDCTL2.DAT.         **
**                                                                          **
**  output:     uses fread() and fwrite() to insert promptfile information  **
**              from the specified prompt file into the data dictionary at  **
**              the specified starting point (e.g., prompt number + 2000).  **
**                                                                          **
**  programmer: Joe Loughry                                                 **
**                                                                          **
**  created:    05/08/93                                                    **
**                                                                          **
**  modified:   05/10/93                                                    **
**                                                                          **
**  notes:      First version converts only prompt names and types.  It     **
**              does not attempt to convert comments (long names), help     **
**              files, or field descriptors.  (These will be added later.)  **
**                                                                          **
**              ...fflush() is required between when switching between      **
**              reading and writing when using fopen in "r+b" (rhythm &     **
**              blues) mode...                                              **
**                                                                          **
**              for simplicity's sake, paths to the BB data dictionary      **
**              files are hard-coded to C:\BANKER\FORMS\????????.DAT.       **
**                                                                          **
**              05-10-93: changed encoding of DATE type prompts to plain    **
**              old alpha to avoid problems with BB "illegal date" error    **
**              messages in converted screens.                              **
**                                                                          **
*****************************************************************************/

int bbdd ( Promptfile prompt, int starting_at ) {
        FILE *fp_ddctl1;                /* BB data dictionary data file 1 */
        struct ddrecordtype {
                char name[8];
                int length;
                char chartest[8];
                char fieldtest[16];
                unsigned char zerobyte;
                long default_index;
                long comment_index;
                long help_index;
                unsigned char endbyte;
        }
        typedef struct ddrecordtype ddrecord;
        ddrecord current_record;
        int record_index = 0;
        int prompt_number = 1;
        int i;
        if ((fp_ddctl1 = fopen(DDCTL1FILE, "r+b")) == NULL) {
                perror("L_BBDD");
                fclose(fp_ddctl1);
                return(-1);
        }
/*
        if ((fp_ddctl2 = fopen(DDCTL2FILE, "r+b")) == NULL) {
                perror("L_BBDD");
                fclose(fp_ddctl2);
                return(-1);
        }
*/
        printf("Converting prompt file to data dictionary...\n");
        /*
        // seek to the beginning of the data dictionary file.
        */
        record_index = starting_at;
        fseek(fp_ddctl1, (long)(record_index) * RECORDSIZE, SEEK_SET);
        /*
        // now work our way through the BANCStar prompt file.
        */
        for (prompt_number=1; prompt_number<=MAX_NUM_PROMPTS; prompt_number++) {
                /*
                // clear out the current_record and load it up with
                // the corresponding information from the prompt file.
                */
/*
This section commented out 05-08-93: we are only updating the file.
                n = fread(&current_record, RECORDSIZE, 1L, fp_ddctl1);
                if (n == 0) {
                        perror("fread() returned zero on fp_ddctl1");
                        fclose(fp_ddctl1);
                        return(-1);
                }
                fflush(fp_ddctl1);
*/
                memset(&current_record, 0, RECORDSIZE);
                sprintf(current_record.name, "%-4.4s%04d",
                                             prompt[prompt_number].name,
                                             prompt_number);
                current_record.length = prompt[prompt_number].length;
                switch(prompt[prompt_number].type) {
                        case P_PHONE:
                                current_record.chartest[0] = 'C';

                                current_record.fieldtest[0] = '3';
                                current_record.fieldtest[1] = '/';
                                break;

                        case P_SSNUMBER:
                                current_record.chartest[0] = 'A';

                                current_record.fieldtest[0] = '2';
                                current_record.fieldtest[1] = '/';
                                break;

                        case P_DATE:
                                current_record.chartest[0] = 'A';

                                current_record.fieldtest[0] = '/';
                                break;
/*
This section commented out 05-10-93
                                current_record.chartest[0] = 'C';

                                current_record.fieldtest[0] = 'A';
                                current_record.fieldtest[1] = '4';
                                current_record.fieldtest[2] = '/';
*/

                        case P_NUMBER:
                                current_record.chartest[0] = 'D';

                                current_record.fieldtest[0] = 'B';
                                current_record.fieldtest[1] = '/';
                                current_record.fieldtest[2] = '0';
                                current_record.fieldtest[3] = '-';
                                for (i=4; i<(prompt[prompt_number].length)+4; i++)
                                        current_record.fieldtest[i] = '9';
                                break;

                        case P_DOLLAR:
                                current_record.chartest[0] = 'D';

                                current_record.fieldtest[0] = 'B';
                                current_record.fieldtest[1] = '/';
                                current_record.fieldtest[2] = '0';
                                current_record.fieldtest[3] = '-';
                                for (i=4; i<(prompt[prompt_number].length)+4; i++)
                                        current_record.fieldtest[i] = '9';
                                break;

                        case P_RATE:
                                current_record.chartest[0] = 'D';

                                current_record.fieldtest[0] = 'B';
                                current_record.fieldtest[1] = '/';
                                current_record.fieldtest[2] = '.';
                                current_record.fieldtest[3] = '0';
                                current_record.fieldtest[4] = '0';
                                current_record.fieldtest[5] = '1';
                                current_record.fieldtest[6] = '-';
                                current_record.fieldtest[7] = '5';
                                current_record.fieldtest[8] = '0';
                                break;

                        case P_ALPHA, P_CONSTANT:
                                current_record.chartest[0] = 'A';

                                current_record.fieldtest[0] = '/';
                                break;

                        default:
                                current_record.chartest[0] = 'A';

                                current_record.fieldtest[0] = '/';
                                break;
                }
                /*
                // later on, we'll comment these out and only update the file.
                */
                current_record.zerobyte = 0x00;
                current_record.default_index = 0x00000000;
                current_record.comment_index = 0x00000000;
                current_record.help_index = 0x00000000;
                current_record.endbyte = 0xA0;
                /*
                // write the current record out to the DD file.
                */
                if (prompt[prompt_number].length != 0) {
                        fseek(fp_ddctl1, (long)(record_index) * RECORDSIZE,
                                                               SEEK_SET);
                        fwrite(&current_record, RECORDSIZE, 1L, fp_ddctl1);
                }
                /*
                // update index to next position in the DD file.
                */
                ++record_index;
        }
        /*
        // close files and return to main program.
        */
        fclose(fp_ddctl1);
        return(0);
}

