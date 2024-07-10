int i = 0;
int off = 8;

void inc(void){
  i += off;
}


int main(void){
  while (1) {
    inc();
  }
}

