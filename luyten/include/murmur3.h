#pragma once

#include <stdint.h>

typedef class CMURMUR3HASH
{
  protected:
    uint32_t    m_h ;
  
    uint32_t Scramble(uint32_t k) ;
  
  public:
    CMURMUR3HASH(uint32_t seed) : m_h(seed) {} ;
    CMURMUR3HASH(uint32_t seed, const uint8_t *data, uint32_t length) ;
    
    void Feed(uint32_t k) ;
    
    uint32_t Finalize(uint32_t len) ;
    
    operator uint32_t () { return m_h; } ; 

} CMURMUR3HASH ;