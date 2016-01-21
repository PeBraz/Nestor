package main

import (
      "fmt"
)

const (
  NES_HEIGHT = 18
  NES_WIDTH = 20 
  MEMORY_SIZE = 65536
) 

type Nes struct{
  memory [MEMORY_SIZE]byte
  pc uint16 //program counter
  sp byte; //wraps from $00 to $ff
  a byte; // accumulator
  x byte; //register x
  y byte; //register y
  p byte; //flags
}



func (n *Nes) immediate(operation func(*uint16)){
  n.memory[2] = 10
  var val uint16 = uint16(n.memory[2])
  operation(&val)
}

func pr(val *uint16){
 fmt.Println(*val)
}
func main(){
  lol := new(Nes)
  fmt.Println(lol.immediate(pr))
}

// implicit points no nil -> uint16
// accumulator points to byte ->  uint16
// immediate points to temporary value ->  uint16
  // var x byte = 10
  // immediate(&x)
// zero page points to memory byte
// zero page,x points to accumulator with value of page + x
// relative - pointer to byte, operations must transform the byte into a signed byte (+127 to -128)
// absolute points to uint16
// indirect 2bytes for location, points to inside memory, where it should read 2 bytes uin16 (2 spaces of array)
// absolutex - points to temp value uint16 of 16bytes + register x -> uint16
// absolutey - points to temp value uint16 of 16bytes + register y -> uint16
// indexed indirect - gives memory index with byte value (zero page) and an offset if register x (wrapping at 255 (use & 0xFF)) -> byte
// indirect indexed - fetch memory pointed to by the byte first and then add it the value of the register y (takes value from mem) -> uint16