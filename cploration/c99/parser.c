/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] Talan Aria
 * [TERM] FALL 2026
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	
	char s_new[strlen(s) + 1];
	int i = 0;

	for (char *s2 = s; *s2; s2++){
		if(*s2 == '/' && *(s2+1) == '/'){
			break;
		}else if(isspace(*s2) == 0){
			s_new[i++] = *s2;
		}
	}
	s_new[i] = '\0';
	strcpy(s,s_new);
    return s;	
}

char *extract_label(const char *line, char *label) {
    int j = 0;

    for (int i = 1; i < strlen(line) - 1; i++) {
        label[j] = line[i];
        j++;
    }

    label[j] = '\0';
    return label;
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */

void parse(FILE * file){
	instruction instr;
	char line[MAX_LINE_LENGTH] = {0};
	int address = 0;
	unsigned int line_num  = 0;
	unsigned int instr_num = 0;
	add_predefined_symbols();
	while(fgets(line,sizeof(line),file)){
		line_num = line_num + 1;
		if(instr_num > MAX_INSTRUCTIONS){
			exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
		}
		strip(line);
		if(line[0] == '\0'){
			continue;
		}
		char inst_type = ' ';
		if(is_Atype(line) == true){
			if (!parse_A_instruction(line, &instr.data.a)){
				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
			}
			instr.type = a_type_instruction;
			inst_type = 'A';
			address++;
			
		}
		else if(is_label(line) == true){
			inst_type = 'L';
			char label[strlen(line) - 1];
			extract_label(line, label);
			if(isalpha(label[0]) == 0){
				exit_program(EXIT_INVALID_LABEL, line_num, label);
			}
			if(symtable_find(label) != NULL){
			    exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, label);
			}
			symtable_insert(label, instr_num);
			continue;
			strcpy(line,label);
		}
		else if(is_Ctype(line) == true){
			inst_type = 'C';
			address++;
		}
		
		//printf("%c  %s\n", inst_type, line);
		//printf("%u: %c  %s\n", instr_num, inst_type, line);
		instr_num = instr_num + 1;
	}
	
}

bool is_Atype(const char *line){
	if(line[0] == '@'){
		return true;
	}else{
		return false;
	}
}

bool is_label(const char *line){
    int length = strlen(line);

    if (line[0] == '(' && line[length - 1] == ')') {
        return true;
    } else {
        return false;
    }
}

bool is_Ctype(const char *line) {
    return !is_Atype(line) && !is_label(line);
}

void add_predefined_symbols() {
	for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++){
		predefined_symbol sym = predefined_symbols[i];
		symtable_insert(sym.name,sym.address);
	}
	
}

bool parse_A_instruction(const char *line, a_instruction *instr){
	char *s = malloc(strlen(line));
	strcpy(s, line+1);
	
	char *s_end = NULL;
	long result = strtol(s, &s_end, 10); 
	
	if(strcmp(s,s_end) == 0){
		instr->label = malloc(strlen(line));
		strcpy(instr->label,s);
		instr->is_addr = false;
	}else if (*s_end != 0){
		return false;
	}else {
		instr->address = result;
        instr->is_addr = true;
	}
	
	return true;
}