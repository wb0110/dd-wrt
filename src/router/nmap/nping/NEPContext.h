 
/***************************************************************************
 * NEPContext.cc --                                                        *
 *                                                                         *
 ***********************IMPORTANT NMAP LICENSE TERMS************************
 *                                                                         *
 * The Nmap Security Scanner is (C) 1996-2012 Insecure.Com LLC. Nmap is    *
 * also a registered trademark of Insecure.Com LLC.  This program is free  *
 * software; you may redistribute and/or modify it under the terms of the  *
 * GNU General Public License as published by the Free Software            *
 * Foundation; Version 2 with the clarifications and exceptions described  *
 * below.  This guarantees your right to use, modify, and redistribute     *
 * this software under certain conditions.  If you wish to embed Nmap      *
 * technology into proprietary software, we sell alternative licenses      *
 * (contact sales@insecure.com).  Dozens of software vendors already       *
 * license Nmap technology such as host discovery, port scanning, OS       *
 * detection, version detection, and the Nmap Scripting Engine.            *
 *                                                                         *
 * Note that the GPL places important restrictions on "derived works", yet *
 * it does not provide a detailed definition of that term.  To avoid       *
 * misunderstandings, we interpret that term as broadly as copyright law   *
 * allows.  For example, we consider an application to constitute a        *
 * "derivative work" for the purpose of this license if it does any of the *
 * following:                                                              *
 * o Integrates source code from Nmap                                      *
 * o Reads or includes Nmap copyrighted data files, such as                *
 *   nmap-os-db or nmap-service-probes.                                    *
 * o Executes Nmap and parses the results (as opposed to typical shell or  *
 *   execution-menu apps, which simply display raw Nmap output and so are  *
 *   not derivative works.)                                                *
 * o Integrates/includes/aggregates Nmap into a proprietary executable     *
 *   installer, such as those produced by InstallShield.                   *
 * o Links to a library or executes a program that does any of the above   *
 *                                                                         *
 * The term "Nmap" should be taken to also include any portions or derived *
 * works of Nmap, as well as other software we distribute under this       *
 * license such as Zenmap, Ncat, and Nping.  This list is not exclusive,   *
 * but is meant to clarify our interpretation of derived works with some   *
 * common examples.  Our interpretation applies only to Nmap--we don't     *
 * speak for other people's GPL works.                                     *
 *                                                                         *
 * If you have any questions about the GPL licensing restrictions on using *
 * Nmap in non-GPL works, we would be happy to help.  As mentioned above,  *
 * we also offer alternative license to integrate Nmap into proprietary    *
 * applications and appliances.  These contracts have been sold to dozens  *
 * of software vendors, and generally include a perpetual license as well  *
 * as providing for priority support and updates.  They also fund the      *
 * continued development of Nmap.  Please email sales@insecure.com for     *
 * further information.                                                    *
 *                                                                         *
 * As a special exception to the GPL terms, Insecure.Com LLC grants        *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
 * linked combinations including the two. You must obey the GNU GPL in all *
 * respects for all of the code used other than OpenSSL.  If you modify    *
 * this file, you may extend this exception to your version of the file,   *
 * but you are not obligated to do so.                                     *
 *                                                                         *
 * If you received these files with a written license agreement or         *
 * contract stating terms other than the terms above, then that            *
 * alternative license agreement takes precedence over these comments.     *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes (none     *
 * have been found so far).                                                *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to nmap-dev@insecure.org for possible incorporation into the main       *
 * distribution.  By sending these changes to Fyodor or one of the         *
 * Insecure.Org development mailing lists, or checking them into the Nmap  *
 * source code repository, it is understood (unless you specify otherwise) *
 * that you are offering the Nmap Project (Insecure.Com LLC) the           *
 * unlimited, non-exclusive right to reuse, modify, and relicense the      *
 * code.  Nmap will always be available Open Source, but this is important *
 * because the inability to relicense code has caused devastating problems *
 * for other Free Software projects (such as KDE and NASM).  We also       *
 * occasionally relicense the code to third parties as discussed above.    *
 * If you wish to specify special license conditions of your               *
 * contributions, just say so when you send them.                          *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * General Public License v2.0 for more details at                         *
 * http://www.gnu.org/licenses/gpl-2.0.html , or in the COPYING file       *
 * included with Nmap.                                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef __NEPCONTEXT_H__
#define __NEPCONTEXT_H__ 1



#include "nsock.h"
#include "EchoHeader.h"
#include <vector>
using namespace std;

/* SERVER STATE MACHINE                                                       */
/*                      _                                                     */
/*                     (O)                                                    */
/*                      |                                                     */
/* Capture Raw /        |                  Rcvd TCP Connection /              */
/* Send NEP_ECHO        |     +--------+   Send NEP_HANDSHAKE_SERVER          */
/*   +----------+       +---->| LISTEN |-----------------+                    */
/*   |          |             +--------+                 |                    */
/*   |         \|/                                      \|/                   */
/*   |     +----+------+                      +----------+-----------+        */
/*   +-----| NEP_READY |                      | NEP_HANDSHAKE_SERVER |        */
/*         |    SENT   |                      |         SENT         |        */
/*         +----+------+                      +----------+-----------+        */
/*             /|\                                       |                    */
/*              |                                        |                    */
/*              |       +---------------------+          |                    */
/*              |       | NEP_HANDSHAKE_FINAL |          |                    */
/*              +-------|        SENT         |<---------+                    */
/*                      +---------------------+   Rcvd NEP_HANDSHAKE_CLIENT/  */
/*  Rcvd NEP_PACKETSPEC /                         Send NEP_HANDSHAKE_FINAL    */
/*  Send NEP_READY                                                            */
/*                                                                            */
/*                                                                            */



#define STATE_LISTEN          0x00
#define STATE_HS_SERVER_SENT  0x01
#define STATE_HS_FINAL_SENT   0x02
#define STATE_READY_SENT      0x03

#define CLIENT_NOT_FOUND -1
typedef int clientid_t; /**< Type for client identifiers */

#define MAC_KEY_S2C_INITIAL 0x01
#define MAC_KEY_S2C         0x02
#define MAC_KEY_C2S         0x03
#define CIPHER_KEY_C2S      0x04
#define CIPHER_KEY_S2C      0x05


/* Client field specifier */
typedef struct field_spec{
  u8 field; /* Field identifier (See NEP RFC) */
  u8 len;   /* Field length */
  u8 value[PACKETSPEC_FIELD_LEN]; /* Field data */
}fspec_t;

class NEPContext {

    private:

        clientid_t id;     /**<  Client identifier */
        nsock_iod nsi;     /**<  Client nsock IOD  */
        int state;
        u32 last_seq_client;
        u32 last_seq_server;
        u8 next_iv_enc[CIPHER_BLOCK_SIZE];
        u8 next_iv_dec[CIPHER_BLOCK_SIZE];
        u8 nep_key_mac_c2s[MAC_KEY_LEN];
        u8 nep_key_mac_s2c[MAC_KEY_LEN];
        u8 nep_key_ciphertext_c2s[CIPHER_KEY_LEN];
        u8 nep_key_ciphertext_s2c[CIPHER_KEY_LEN];
        u8 server_nonce[NONCE_LEN];
        u8 client_nonce[NONCE_LEN];
        bool server_nonce_set;
        bool client_nonce_set;
        vector<fspec_t> fspecs;
        struct sockaddr_storage clnt_addr;

        u8 *generateKey(int key_type, size_t *final_len);
        
    public:

        NEPContext();
        ~NEPContext();
        void reset();

        int setIdentifier(clientid_t clnt);
        clientid_t getIdentifier();

        int setAddress(struct sockaddr_storage a);
        struct sockaddr_storage getAddress();

        int setNsockIOD(nsock_iod iod);
        nsock_iod getNsockIOD();

        int setState(int state);
        int getState();

        bool ready();

        int setNextEncryptionIV(u8 *block);
        u8 *getNextEncryptionIV(size_t *final_len);
        u8 *getNextEncryptionIV();

        int setNextDecryptionIV(u8 *block);
        u8 *getNextDecryptionIV(size_t *final_len);
        u8 *getNextDecryptionIV();

        int setLastServerSequence(u32 seq);
        u32 getLastServerSequence();
        u32 getNextServerSequence();
        int setLastClientSequence(u32 seq);
        u32 getLastClientSequence();
        u32 getNextClientSequence();
        int generateInitialServerSequence();
        int generateInitialClientSequence();

        int setMacKeyC2S(u8 *key);
        u8 *getMacKeyC2S();
        u8 *getMacKeyC2S(size_t *final_len);
        int generateMacKeyC2S();

        int setMacKeyS2C(u8 *key);
        u8 *getMacKeyS2C();
        u8 *getMacKeyS2C(size_t *final_len);
        int generateMacKeyS2C();

        int setCipherKeyC2S(u8 *key);
        u8 *getCipherKeyC2S();
        u8 *getCipherKeyC2S(size_t *final_len);
        int generateCipherKeyC2S();
        int generateMacKeyS2CInitial();

        int setCipherKeyS2C(u8 *key);
        u8 *getCipherKeyS2C();
        u8 *getCipherKeyS2C(size_t *final_len);
        int generateCipherKeyS2C();
        
        int generateClientNonce();
        int generateServerNonce();
        int setClientNonce(u8 *buff);
        u8 *getClientNonce();
        int setServerNonce(u8 *buff);
        u8 *getServerNonce();

        int addClientFieldSpec(u8 field, u8 len, u8 *value);
        fspec_t *getClientFieldSpec(int index);
        bool isDuplicateFieldSpec(u8 test_field);
        int resetClientFieldSpecs();

};

#endif /* __NEPCONTEXT_H__ */
