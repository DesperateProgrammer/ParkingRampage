#include "murmur3.h"


CMURMUR3HASH::CMURMUR3HASH(uint32_t seed, const uint8_t *data, uint32_t length) 
: m_h(seed)
{
  uint32_t c ;
  uint32_t pos = 0 ;
  while (pos < length)
  {
    c = 0 ;
    for (uint32_t i=0;i<4;i++)
    {
      if ((pos + i) < length)
        c = ((uint32_t)data[pos + i]) << (1 << (i*8)) ;
    }
    Feed(c) ;
    pos += 4 ;
  }
  Finalize(length) ;
}

uint32_t CMURMUR3HASH::Scramble(uint32_t k) 
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

void CMURMUR3HASH::Feed(uint32_t k)
{
  m_h ^= Scramble(k);
  m_h = (m_h << 13) | (m_h >> 19);
  m_h = m_h * 5 + 0xe6546b64;
}

uint32_t CMURMUR3HASH::Finalize(uint32_t len) 
{
  /* Finalize. */
	m_h ^= len;
	m_h ^= m_h >> 16;
	m_h *= 0x85ebca6b;
	m_h ^= m_h >> 13;
	m_h *= 0xc2b2ae35;
	m_h ^= m_h >> 16;
  return m_h ;
}
