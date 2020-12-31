#include <stdint.h>

#include <string>
#include <vector>

#define TAG_FLAG_RECURSIVE          0x80000000

typedef class CTLV
{
  protected:
    std::string m_leaf ;
    std::vector<class CTLV *> m_childs;
    uint32_t m_tag ;
    
  public:
   CTLV(uint32_t tag) ;
   CTLV(std::string serializedTLV) ;
   
   bool SetData(uint8_t *src, uint32_t length) ;
   bool AddChild(class CTLV *tlv) ;
   bool RemoveChild(class CTLV *tlv) ;
   
   std::string Serialize() ;
   
   uint32_t GetTag() { return m_tag; } ;

    uint32_t GetChildCount() ;
    CTLV *GetChild(uint32_t index) ;
    
    
    void SetUInt32(uint32_t value) ;
    uint32_t GetUInt32() ;
    
} CTLV ;