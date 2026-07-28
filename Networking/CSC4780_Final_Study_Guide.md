**CSC 4780 — Computer Networks and Security**  
   
   **Final Exam Study Guide**  
   
 ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAMUlEQVR4nO3WAQkAIBAEsBMsY8vPZRUDWMAA5njYUmxU1UqyAwBAF2cmeZE4AIBO7genygXZ8QrWRQAAAABJRU5ErkJggg==)  
   
    
   
   
   **1. BGP — Border Gateway Protocol**  
   
   
   **What problem does BGP solve, and why does that design create security risks?**  
- The Internet is a network of **Autonomous Systems (ASes)** — each a set of routers under one administrative domain.  
- Inside an AS: **IGP** (Interior Gateway Protocol). Between ASes:     **BGP**.  
- BGP goal: **reachability, not optimality** — find *any* loop-free path to a destination.  
- BGP is a **path-vector protocol**: every update carries the full AS-path, so loops are detected if an AS sees itself in the path.  
- BGP allows **policy**: an AS can choose what paths to accept, prefer, or advertise. This is configuration, not part of the protocol.  
- **Hop-by-hop model**: you can only advertise what you are actually using; you control what you tell neighbors.  
- AS types: **stub** (one connection, no transit),     **multihomed** (multiple connections, no transit),     **transit** (carries traffic for others).  
- **BGP messages**: OPEN, UPDATE, NOTIFICATION, KEEPALIVE — run over TCP.  
- **RIB**: Adj-RIB-In (from peers) → RIB-Local (best path selection) → Adj-RIB-Out (advertised to peers).  
   
   
   **Questions**  
1. Why does BGP prioritize reachability over optimality? What would go wrong if it tried to find the *optimal* path?  
- **BGP is a collection of independent ASes each has its own policies. Finding the optimal route would require strict standards and policy enforcement. They each have their own goals so any global policy would be messy.**   
   
    
   
     
   
    
   
      
1. A stub AS has one upstream provider. Does it need to run BGP? Why or why not?  
- **No it will not need an upstream provider because the default route is to only one it needs. It may need BGP to advertise its route.**     
1. You are a multi-homed AS that does not want to carry traffic between your two upstream providers. What do you tell each of them, and why?  
- **You just need to make sure that you don’t advertise routes from one provider to another. Tell provider A to only use your prefixes and tell provider B the same thing. This will ensure that the ASes do not mix traffic.**  
   
    
1. How does the path-vector mechanism prevent routing loops? Walk through the mechanism step by step.  
- **Path-vector algorithm share the entire path with each AS so when an AS sees itself it knows that there is a loop and drops that path. Otherwise it will just itself and forward to the next AS**  
   
     
1. What is the difference between what BGP *can* enforce and what it     *cannot* enforce without configuration?  
- **BGP can only enforce local policy decisions because each one is independent. They cannot enforce anything globally because BGP assumes trust and there is no authentication.** **  
   
  **  
   
     
   
  ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAM0lEQVR4nO3OMQ0AIAwAwdLgBKMVgjmcsGCAiZDcTT9+q6oRETMAAPjF6ify6QYAADdyA9ZAAyq60E8DAAAAAElFTkSuQmCC)  
   
    
   
     
**2. BGP Security — RPKI and BGPSEC**  
   
    
   
   **Why is BGP insecure, and what do RPKI and BGPSEC each fix?**  
- BGP has been in use since 1994 — security was never built in.  
- BGP **trusts all advertisements from peers with no cryptographic verification**.  
- **Prefix hijacking**: AS announces a prefix it does not own → traffic is redirected or blackholed.  
- **Sub-prefix hijacking**: attacker announces a *more specific* prefix — routers prefer more specific routes, so the attacker wins.  
- **Route leak**: an AS propagates routes beyond intended scope (e.g., a stub accidentally becomes a transit provider).  
- **RPKI (Resource Public Key Infrastructure)**:  
- Signs *origin* of route advertisements.  
- Five Regional Internet Registries (RIRs) serve as trust anchors: ARIN, APNIC, RIPE, LACNIC, AFRINIC.  
- Each AS creates a **Route Origin Authorization (ROA)**: authorized AS number + prefix + maxLength.  
- Validators assign each route one of three states: **VALID, INVALID, NOT FOUND**.  
- RPKI does NOT validate the full path — only who originated the prefix.  
- **BGPSEC**: validates the AS path (each AS in the path signs it cryptographically). Built on top of RPKI.  
- Operational risks: incorrect ROA causes legitimate outages; fail-open vs fail-closed is a design tradeoff.  
   
    
   
   **Questions**  
1. Pakistan Telecom announced YouTube's prefix in 2008. What type of attack is this? How would RPKI have prevented it?  
- This was a prefix attack where Pakistan announced YouTube’s IP and traffic got rerouted. RPKI would have marked this announcement as invalid and the routers would have rejected it.   
1. RPKI protects the *origin*. Why is that not sufficient? What attack does BGPSEC additionally prevent?  
- **The origin protection only verifies who owns the prefix it is not able to verify the path. Attackers can still insert themselves into the path. BGPSec fixes this by signing the entire path. **  
1. An ISP misconfigures a route and leaks it to the wider Internet. Is this a hijack? Is RPKI helpful here?  
- **A route leak is not considered an attack because it is usually an accident. A hijack is a malicious and intentional attempt to change a legitimate route. RPKI does not help with the leaks because it only offers origin protection not route confirmation.**   
   
    
   
     
1. What does "fail-open" mean in the context of RPKI? What is the tradeoff between fail-open and fail-closed?  
- **The fail-open model means that invalid routes are still accepted to preserve connectivity. Fail-closed rejects them but you run into issues with outages losing availability in exchange for better security.**   
      
1. Why do the RIRs serve as trust anchors rather than, say, ICANN or a private company?  
- **Regional Internet Registries (ARIN, APNIC) act as trust anchors because they alredy manage IP address allocation making them the best authority for managing and validating ownership. A private company may have conflicts of interest in managing access.**  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANklEQVR4nO3OQQmAABRAsScYwLvtzGCAX9MItrCCNxG2BFtmZq/OAAD4i3uttur4egIAwGvXA7/GBEnY4XR2AAAAAElFTkSuQmCC)  
   
    
   
   **3. DNS and Security**  
   
    
   
   **How does DNS work, and what makes it vulnerable?**  
- DNS maps names → IP addresses; replaces the old centralized hosts.txt system.  
- **Hierarchical, distributed, delegated**: root → TLDs → second-level domains → subdomains.  
- **Zones**: each administrator controls their portion; ICANN controls root.  
- **Resolution**: iterative (client does the work) or recursive (resolver does the work).  
- **Caching**: responses cached with a TTL. Negative results cached too.  
- **Fundamental vulnerability**: no authentication in original DNS. Resolvers trust any response received after sending a query.  
- **Cache poisoning**: attacker races to reply before the real nameserver. Wins by guessing the 16-bit Transaction ID (TXID).  
- Birthday attack (Vagner Sacramento, 2002): flood many clients with the same query + random TXIDs simultaneously → ~50% success with 300 replies.  
- **Kaminsky attack (2008)**: query a random subdomain of a target domain; send hundreds of forged replies with random TXIDs; a successful poison injects a malicious nameserver record, not just one record.  
- **DNS Rake**: named after lockpicking; sends forged replies with NS redirections to a fake server; returns to step 1 if it fails; cached until TTL expires.  
- **Bailiwick rules**: a nameserver can only return records within its own zone — prevents foo.com from injecting records for google.com.  
- **DNSSEC**: adds cryptographic signatures to DNS records; protects integrity but not confidentiality.  
   
  **Questions**  
1. Why did hosts.txt fail? What specific properties of the Internet made a distributed system necessary?  
- **The file approach failed as the internet grew larger and the file management did not scale as it relied on a central file that had no way to delegate the distribution.**   
   
    
   
     
1. A resolver has a cached record with a TTL of 3600 seconds. Why might a long TTL be dangerous? Why might a short TTL also be dangerous?  
- **A long TTL might be dangerous because if it is incorrect or poisoned it stays an issue for longer and causes more problems. A short TTL means more DNS traffic and opens the router to more attacks.**  
1. Explain the Kaminsky attack. Why is querying a *random subdomain* more powerful than querying the actual target record?  
- **Kaminsky attacks focusing on querying random subdo** **ma** **ins (1.google.com) forcing the resolver to ask repeatedly giving the attacker more chances to guess the TXID and poison the entire DNS record. This is a more powerful attack because their is no need to wait for the TTL to expire on a legitimate request.**  
1. The bailiwick rule prevents a .com nameserver from injecting records for .edu. Why is this insufficient against all cache poisoning attacks?  
- **This rule will prevent cross domain attacks but it does nothing for in-domain attacks.**     
1. DNSSEC protects integrity. Does it protect confidentiality? Why does the distinction matter?  
- **DNSSEC provides cryptogra**phic signatures to ensure the integrity of the DNS responses but the attacker can still see which domains are being queried. DNSSEC is only confirming that the origins are correct an attacker can still learn a lot of information just from the metadata.   
    
   
     
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANklEQVR4nO3OMQ2AABAAsSPBCj7fDLowwIAABiywEZJWQZeZ2ao9AAD+4liruzq/ngAA8Nr1ABwqBgfOW44LAAAAAElFTkSuQmCC)  
   
    
   
     
   
    
   
   **4. PKI — Public Key Infrastructure**  
   
    
   
   **How do we establish trust in a public key we have never seen before?**  
- **Symmetric key**: same key for encryption and decryption — fast but requires pre-shared secret. O(n²) key exchanges for n users.  
- **Public key (asymmetric)**: separate encryption (public) and decryption (private) keys — solves key distribution but is slow.  
- **Digital signatures**: sign with private key (K⁻), verify with public key (K⁺). Only the signer can produce a valid signature.  
- **Combining them**: symmetric crypto for bulk data; public key crypto to exchange the symmetric key; digital signatures to validate public keys.  
- **The trust problem**: how does Alice know that a key she downloads actually belongs to Bob?  
- **Certificate**: public key + owner's name + validity dates + other fields,     **digitally signed by a Certificate Authority (CA)**.  
- A certificate says: *"I, the CA, certify that this public key belongs to this person."*  
- **Certificate issuance**: user generates key pair, proves identity to CA, CA signs the certificate.  
- **Distribution**: certificates can be distributed over any channel — anyone can verify the CA's signature.  
- **Revocation**: Certificate Revocation List (CRL); or use short expiration times.  
- **Trust hierarchy**: Root CA → Intermediate CAs → End-entity certificates. Trust is delegated downward.  
- **PGP model**: trust via personal relationships ("web of trust") rather than hierarchical CAs.  
   
    
   
   **Questions**  
1. Why does symmetric key exchange require O(n²) meetings but a key server requires only O(n) meetings? What security risk does the key server introduce?  
- **Symetric key systems require each pair of users to share a unique key but a key server allows each user to publish a single public key. This creates a problem of needing to trust a central key authority**  
1. If public key encryption is much slower than symmetric encryption, why use it at all?  
- **Public key cryptography is used because it solves the issue of needing complex protocols for exchanging symmetric keys. It is mainly used to establish the symmetric session keys.**  
1. What does a certificate *actually* prove? What does it not prove?  
- **A certificate proves that a public key is associated with a specific identity verified by a Certificate Authority, it does not guarantee that the entity is trustworthy just that they are who they say they are.**  
1. Alice downloads Bob's certificate from Bob's own website. Is this trustworthy? Why or why not?  
- **This is not secure because it is susceptible to a MITM attack and Alice cannot confirm Bobs identity. That is why a trusted third party is needed.**  
1. What is the difference between a certificate expiring and a certificate being revoked? Which is safer and why?  
- **A certificate expiring is passive safety mechanism based on time where a revocation means the certificate is actively invalidated before it expires. This is harder to enforce**  
1. If a Root CA is compromised, what is the blast radius?  
   
  **The entire trust hierarchy would be affected which would allow attackers to impersonate anyone**  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSPBCj5fDbbYGVDAgAU2QtIq6DIzW7UHAMBfHGt1V+fXEwAAXrseHDAGBOVJhdIAAAAASUVORK5CYII=)  
   
    
   
   **5. One-Time Pad and Hash Functions**  
   
    
   
   **What is perfect secrecy, and why is it impractical?**  
- **Goal of encryption**: confidentiality against a passive eavesdropper.  
- **One-Time Pad (OTP)**: key is a truly random bit string equal in length to the message; XOR the key with the plaintext; key used     **exactly once**.  
- OTP provides **perfect secrecy** (information-theoretically secure): ciphertext reveals *nothing* about the plaintext, even with unlimited computation. Formally: P(M=m) = P(M=m | C=c).  
- OTP does **not** hide message length.  
- **Two-time pad failure**: if the same key is reused for two messages, C1 ⊕ C2 = M1 ⊕ M2 — both messages are revealed.  
- **Computationally secure vs. information-theoretically secure**: computational security assumes attackers have limited computing power; information-theoretic security makes no such assumption.  
- **Hash functions**: map arbitrary-length input to fixed-length output. Properties required: pre-image resistance, second pre-image resistance, collision resistance.  
- Three infrastructure security mechanisms: DNSSEC (name integrity), BGPSEC (routing integrity), PKI (identity). Encryption protects the *content*.  
   
    
   
   **Questions**  
1. An attacker intercepts C and tries every possible key. Why can't they determine which plaintext is correct? What does this tell you about the nature of perfect secrecy?  
- **The plaintext is the same size as the message so that any decrypted plaintext message is equally likely with no way of telling which one is the original message**  
1. Two encrypted messages are sent with the same OTP key. Show why XOR-ing the ciphertexts reveals information about the plaintexts.  
- **If the same key is used then C1​⊕C2​=M1​⊕M2. XORing the two ciphertexts will reveal the XOR of the plaintexts which the attacker can use to recover both messages.**  
1. OTP is perfectly secure. Why does no real system use it?  
- **OTP is impractical because it requires a truly random key equal in length to the message and it cannot ever be reused making it difficult to use in practice**  
1. What is the difference between "computationally secure" and "information-theoretically secure"? Give an example of each.  
- **Computational security (AES), assumes limited attacker resources, meaning that its possible to break it but computers haven't become powerful enough. Information-theoretically secure (OTP) holds even against unlimited computation.**  
1. Why does OTP not hide message length, and why does that matter?  
- **The message length is not hidden because the message and key have to be the same length and this gives you information about the message itself that can help to break it**  
   
    
 **6. Secret-Key (Symmetric) Cryptography**  
   
    
   
   **How does DES work, and why was it replaced?**  
- **Block ciphers**: encrypt fixed-size blocks (64 bits for DES, 128 bits for AES). Stronger analysis, broader use.  
- **Stream ciphers**: encrypt one bit/byte at a time.  
- **Shannon's principles (1949)**: *confusion* (hides key-ciphertext relationship) +     *diffusion* (spreads plaintext influence across ciphertext). Both are necessary.  
- **Feistel structure**: split block into L and R halves; each round: new R = L ⊕ F(R, subkey); swap halves. Decryption uses same structure with reversed subkeys.  
- **DES**: 64-bit block, 56-bit effective key, 16 Feistel rounds.  
- Building blocks: Initial Permutation (IP) → Feistel rounds → Final Permutation (IP⁻¹).  
- Each round: expand R (32→48 bits), XOR with subkey, 8 S-boxes (6→4 bits each, nonlinear, source of confusion), P-box permutation (diffusion).  
- S-boxes provide all nonlinearity — without them, DES would be a linear cipher.  
- IP provides no cryptographic security — only hardware efficiency.  
- **DES weaknesses**: 56-bit key broken by brute force in 56 hours (EFF Deep Crack, 1998).  
- **NSA controversy**: key reduced from 64 to 56 bits; S-boxes modified — later revealed to be hardened against differential cryptanalysis.  
- **AES**: 128-bit block, 128/192/256-bit keys; fast in hardware and software; current standard.  
- **3DES**: applies DES three times with different keys — compatibility bridge, now deprecated.  
   
    
   
   **Questions**  
1. What is the difference between confusion and diffusion? Give a concrete example of each from DES.  
- **Confusion seeks to obscure the relationship between the key and ciphertext DES uses S-boxes. Diffusion spreads the letters around the plaintext through permutations to make it harder to find words with pairs of letters like hello**  
1. Why does the Feistel structure allow decryption to use the same hardware/code as encryption?  
- **For encryption/decryption the subkeys are used in reverse order following the inverse of the permutation and compression.**  
1. DES has 8 S-boxes. What would happen to DES security if the S-boxes were replaced with linear functions?  
- **Replacing the S-boxes with linear functions DES would become much easier to break as the confusion would be lost and it would be trivial to calculate the swaps**  
1. The DES Initial Permutation provides no cryptographic security. Why include it at all?  
- **The initial permutation provides no security but the diffusion has an avalanche effect through the rest of the process**  
1. 56 hours to break DES with 1998 hardware. What does that imply for security today? How many bits is "enough"?  
- **Modern keys now need to be at least 128-bits in length to be safe against brute force attacks this provides a search space of 2^ 128****  
1. Why is 3DES considered safer than single DES even though it uses the same underlying primitive?  
- **3DS increases the key length through multiple encryption passes making it more secure but still not as secure as AES**  
   
  ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OQQmAABRAsSeYxZw/kJHEkyk8WcGbCFuCLTOzVXsAAPzFuVZ3dXw9AQDgtesB/wsF8ET8gI4AAAAASUVORK5CYII=)  
   
    
   
   **7. Random Numbers**  
   
    
   
   **Why does randomness matter in cryptography, and what makes a good random number generator?**  
- Cryptographic keys, session tokens, nonces, and stream ciphers all depend on unpredictable random numbers.  
- **The Debian bug (2006–2008)**: only ~32,000 SSH keys were possible instead of millions — attackers could precompute all of them.  
- **True Random Number Generators (TRNGs)**: use physical entropy sources (mouse movement, keyboard timing, electrical noise, CPU noise, fan vibration, network interrupts, lava lamps). Slow but truly random.  
- **Pseudo-Random Number Generators (PRNGs)**: mathematical functions seeded with a random value. Same seed → same output. Fast, but security depends entirely on the seed.  
- LCG: Xₙ₊₁ = (aXₙ + c) mod m — weak, predictable.  
- Blum Blum Shub: Xₙ₊₁ = Xₙ² mod n — secure but slow; based on hardness of factoring.  
- AES-CTR: counter mode AES — fast and cryptographically strong.  
- **Three PRNG requirements**: randomness (no obvious pattern), unpredictability (knowing past output doesn't help predict future), and a     **secure seed** (hard to guess starting value).  
- **Weak seed attack**: time-based seeds are predictable. Attacker narrows guesses to a small window around the likely timestamp.  
- **Unpredictability has two directions**: forward (can't predict future from past) and backward (can't recover past from present).  
- **Real systems**: collect entropy → seed → PRNG → cryptographic keys. Physical randomness is bottleneck; PRNG amplifies it.  
- NIST provides standardized statistical tests for randomness.  
   
    
   
   **Questions**  
1. Why is a random-*looking* sequence not the same as a random sequence? Give an example.  
2. A server seeds its PRNG with the Unix timestamp at startup. Describe an attack. How many guesses does the attacker need?  
3. What is the difference between a TRNG and a PRNG? Why do real systems use both?  
4. Why is it catastrophic to reuse a nonce in stream cipher encryption?  
5. The Debian bug reduced 2^17 possible keys to ~32,000. How would you exploit this in practice?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSPBCj5fDq7YGNDAgAU2QtIq6DIzW7UHAMBfHGt1V+fXEwAAXrseHDwF/g35i4YAAAAASUVORK5CYII=)  
   
    
   
   **8. SSH — Secure Shell**  
   
   
 **What does SSH protect against, and how does it establish a secure channel?**  
- Legacy protocols (Telnet, rsh, rlogin) transmit in **cleartext** and authenticate by IP address or reusable password — trivially interceptable.  
- **SSH protects**: authentication (server and client), confidentiality, integrity, and optionally compression.  
- **Applications**: remote login, remote command execution, file transfer (scp/sftp/rsync), port forwarding, X11 forwarding, tunneling.  
- SSH-2 has **three layers** (all in RFCs):  
1. **Transport Layer**: key exchange, server authentication, encryption, integrity, compression. Runs over TCP port 22.  
2. **User Authentication Layer**: client proves identity to server (password, public key, etc.).  
3. **Connection Layer**: multiplexes logical channels over one SSH connection (shell sessions, port forwards, X11).  
- **Session establishment**: TCP connect → version string exchange → algorithm negotiation → key exchange (Diffie-Hellman) → encrypted data exchange.  
- **Server authentication**: server presents public key; client verifies signature. First-time connection problem: client has no prior key.  
- Most common solution: **TOFU (Trust On First Use)** — save the key, warn if it changes later.  
- Secure solutions: local database of server keys, or CA-signed server certificates.  
- **Key exchange**: Diffie-Hellman (group1-sha1, group14-sha1 required). Both sides derive shared secret without transmitting it.  
- **Encryption**: 3DES-CBC (required), AES-128-CBC (recommended). Minimum 128-bit keys.  
- **Integrity**: HMAC-SHA1 (required).  
   
    
   
   **Questions**  
1. Telnet sends passwords in cleartext. What specific attack does this enable on a shared network?  
2. SSH does key exchange on every connection. Why not just use a pre-shared symmetric key?  
3. You SSH to a server for the first time and get a fingerprint prompt. If you accept without verifying, what attack are you vulnerable to?  
4. What is the Trust On First Use (TOFU) model? What does it protect against and what does it not protect against?  
5. SSH's connection layer multiplexes multiple channels over one TCP connection. What problem does this solve?  
6. Compare SSH to TLS/HTTPS. What security properties do they share? Where do they differ?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANElEQVR4nO3OQQmAABRAsad4ElOY9Mcwn0ms4E2ELcGWmTmrKwAA/uLeqrU6vp4AAPDa/gDzZAM6+oN/0gAAAABJRU5ErkJggg==)  
   
    
   
   **9. QUIC and Modern Transport Security**  
   
    
   
   **Why was QUIC designed, and what security properties does it provide?**  
- Modern Internet facts: >90% of traffic is encrypted; >70% sourced from cloud; >20% uses QUIC.  
- **TCP limitations that motivated QUIC**:  
- Head-of-line blocking (one lost packet stalls all streams).  
- Transport control parameters (window size, etc.) sent in cleartext → manipulation possible.  
- TLS is a second step/afterthought on top of TCP — adds latency.  
- TCP handshake (1.5 RTT) + TLS handshake (1-2 RTT) = slow connection setup.  
- **QUIC**: built on UDP; implements all transport functions (reliability, congestion control, encryption, multi-stream) in the UDP payload at the application layer.  
- **QUIC = TLS over UDP substrate**. In HTTP/3, QUIC replaces TCP+TLS.  
- **Security properties of QUIC**:  
- Everything is encrypted — no visible transport control parameters, no Server Name Indication in plaintext, no visible traffic profile.  
- Connection IDs allow session resumption across network changes (e.g., switching from WiFi to cellular).  
- Eliminates TCP reset attacks and other TCP manipulation attacks.  
- **Setup latency**: QUIC achieves 0-RTT or 1-RTT connection + key establishment vs. TCP+TLS which requires multiple round trips.  
- **Multi-stream**: independent streams in one connection — losing a packet in stream A does not block stream B.  
- **Congestion control**: QUIC supports the same algorithms (Reno, CUBIC, BBR) as TCP but implemented in the application.  
- QUIC encrypts everything — even the handshake — making traffic analysis much harder.  
   
   
  **Questions**  
1. TCP has been the dominant transport protocol since the 1970s. Why would engineers bother building something new instead of fixing TCP?  
2. TCP's control parameters are sent in cleartext. Why is that a security problem? Give a concrete attack.  
3. QUIC encrypts its handshake. What does this prevent that TLS over TCP does not?  
4. Head-of-line blocking in TCP: explain the problem and how QUIC's multi-stream model avoids it.  
5. QUIC is implemented in user space (as part of the application). What are the tradeoffs of this vs. kernel-space TCP?  
6. If QUIC uses UDP, does it sacrifice reliability? Explain.  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANUlEQVR4nO3OMQ2AABAAsSNhYEUBJhD4G0rxgQU2QtIq6DIzR3UFAMBf3Gu1VefXEwAAXtsfSqYDXld2hUQAAAAASUVORK5CYII=)  
   
    
   
   **10. TOR and Anonymity**  
   
    
 **What is anonymity, and how does TOR provide it?**  
- **Anonymity**: hiding the identity of communicating parties from each other or from third parties.  
- Why anonymity: protection against profiling, censorship-resistant publishing, whistleblowing, journalism, abuse victim safety. *Anonymity is not only for criminals.*  
- **Why it's hard**: IP address is tied to identity; ISPs retain communication logs; cookies, browser fingerprinting, and behavioral tracking exist at the application layer; Tier-1 networks may be monitored.  
- **Chaum's Mix-Net**: foundational idea. Messages wrapped in layers of public key encryption pass through a series of "mixes" that decrypt, delay, and reorder messages. Each mix knows only its predecessor and successor.  
- **Anonymity types**: sender anonymity, receiver anonymity, sender-receiver unlinkability.  
- **Properties**: *unlinkability* (cannot link events/packets/people),     *unobservability* (items are indistinguishable from background traffic).  
- **Anonymity set**: the group of people among whom a sender could plausibly be. Larger set = stronger anonymity.  
- **Why proxies/VPNs fail**: the proxy knows both source and destination. If it is compromised or cooperates with authorities, anonymity is broken. Also, application-layer identifiers (cookies, logins) can deanonymize even with network anonymity.  
- **TOR (Onion Routing)**: 3-hop circuit of relays; each hop knows only the previous and next hop; encryption is layered (like an onion) — each relay decrypts one layer.  
- **Limitations**: exit node sees plaintext; timing attacks can correlate entry and exit traffic; content-based deanonymization (e.g., using Google while on TOR).  
- **Protecting all layers**: network anonymity must be complemented by application-layer privacy.  
   
    
   
   **Questions**  
1. SSL/TLS encrypts content but provides no anonymity. Why? What does it protect vs. what it does not?  
2. A VPN routes all traffic through a single gateway. Why does this not provide anonymity?  
3. In TOR, the entry node knows who you are but not where you're going. The exit node knows where you're going but not who you are. Why is the separation important? What attack breaks it?  
4. What is a timing attack against TOR? Why is it hard to defend against?  
5. You use TOR and then log into your Google account. Are you anonymous? Why or why not?  
6. Why must an anonymity system protect at *all* layers, not just the network layer?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANklEQVR4nO3OMQ2AABAAsSNhRgGqsMPyOlGADCywEZJWQZeZ2aszAAD+4l6rrTq+ngAA8Nr1AKLDBD94ZDwzAAAAAElFTkSuQmCC)  
   
    
   
   **11. Quantum Cryptography**  
   
    
   
   **What threat do quantum computers pose to current cryptography, and what defenses exist?**  
- **Quantum computers** operate on     **qubits** that can exist in superposition (both 0 and 1 simultaneously).  
- N qubits → 2ᴺ states simultaneously. A 64-qubit machine operates in a space of ~16 × 10¹⁸ numbers.  
- Doubling power: classical computer needs 2× bits; quantum computer needs only 1 more qubit.  
- **Shor's algorithm**: factors large integers in polynomial time. Breaks RSA, Diffie-Hellman, and elliptic curve cryptography — all public key systems based on factoring or discrete log.  
- **Grover's algorithm**: searches an unstructured database of N entries in O(√N) queries vs. O(N) classically. Effectively halves symmetric key length — 256-bit AES becomes as hard as 128-bit AES against a quantum attacker.  
- **Quantum communication (QKD)**: uses quantum properties of photons to distribute keys. Any eavesdropping disturbs the quantum state and is detectable. BB84 protocol: Alice sends polarized photons; Bob measures with random bases; they compare bases publicly; matching results form the shared key.  
- **Post-quantum cryptography**: classical algorithms designed to resist quantum attacks (lattice-based, hash-based, code-based). NIST has standardized several.  
- **Types of quantum computers**: quantum annealer (D-Wave, optimization only, unproven advantage), analog quantum (~50-100 qubits, special simulations), universal quantum (full general purpose — still in development).  
- Quantum computers will NOT replace classical computers for everyday tasks.  
   
    
   
   **Questions**  
1. RSA security relies on the difficulty of factoring large numbers. Why does Shor's algorithm break this assumption?  
2. Grover's algorithm only halves the effective key length. Why is this less catastrophic than Shor's algorithm for symmetric cryptography?  
3. Why does eavesdropping on a quantum channel always disturb the signal? What physical principle underlies this?  
4. "Harvest now, decrypt later" — what is this attack? What types of data are most at risk?  
5. If quantum computers are not yet powerful enough to break RSA, why should we start worrying now?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANklEQVR4nO3OMQ2AABAAsSNhRgGyUMP0OlGADCywEZJWQZeZ2aszAAD+4l6rrTq+ngAA8Nr1AKK7BECnHclUAAAAAElFTkSuQmCC)  
   
    
   
   **12. Email Security**  
   
    
   
   **What are the security challenges specific to email, and why has end-to-end encryption failed to take hold?**  
- Email encryption standards are 30+ years old (PEM, PGP/GPG, S/MIME) — none is widely used.  
- **Why e2e email encryption failed**: key discovery (hard to find Bob's public key), multi-device private key management, forgotten password destroys all mail, companies need plaintext access for compliance, network effect (useless if contacts don't support it), user apathy.  
- **Store and forward**: email uses Message Transfer Agents (MTAs); direct device-to-device email fails because both devices must be online simultaneously, client devices lack stable IPs, NAT blocks inbound connections.  
- **BASE64**: email was designed for 7-bit ASCII; binary data must be encoded (BASE64: 6 bits → 1 ASCII char, ~33% expansion) to survive legacy infrastructure.  
- **HTML email**: tracks when email is opened (recipient-specific URLs), can execute malicious code if HTML bugs exist in the client, links can be malicious.  
- **Attachments**: .exe always dangerous; .docm dangerous (macros); .docx/.pptx should be safe but have exploitable bugs; PDFs generally safe but can exploit reader vulnerabilities.  
- **Spam**: unwanted bulk email. Defenses: IP denylist (cloud IP reuse harms innocent), keyword/AI filtering (spammers adapt), volume detection, spam folder instead of deletion.  
- **Phishing defense**: MTAs rewrite links to pass through a protection service that scans for malware before the user reaches the destination.  
- **DLP (Data Loss Prevention)**: MTAs scan outbound email for sensitive keywords, credit card numbers, confidential markings.  
- **Distribution lists**: remote exploder (one copy over WAN, parallelism) vs. local exploder (sender knows all recipients). Both must handle nested lists and cycle detection.  
   
    
   
   **Questions**  
1. PGP has existed since 1991. Why is it not universally used for email? What is the single biggest practical obstacle?  
2. An MTA sits between Alice and Bob. What security properties does it provide? What properties does it undermine?  
3. Why is BASE64 necessary for email attachments if modern infrastructure supports binary data?  
4. You receive an HTML email from your bank with a link to verify your account. What specific attacks are possible through that link?  
5. A company wants to scan all encrypted email for malware. Is this compatible with end-to-end encryption? What is the tradeoff?  
6. What is the difference between spam filtering and phishing protection? Can the same system handle both?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAOUlEQVR4nO3OQQmAQAAAwRVsY8nLYSJDXAEfRhCs4E+EmQSzjDG26ggAgD+4q7lWV7V/nAEA4L3zARsLBsK+AKL8AAAAAElFTkSuQmCC)  
   
    
   
   **13. Miscellaneous Cryptographic Techniques**  
   
    
   
   **What do secret sharing, blind signatures, homomorphic encryption, and perfect forward secrecy each solve?**  
- **Secret Sharing (Shamir's scheme)**: secret S is the constant in a degree-(k-1) polynomial. Shares are points on the curve. Any k shares reconstruct the secret. Fewer than k shares reveal *nothing*. Prevents single point of failure.  
- **Blind Signatures**: message is blinded with a random factor before signing; signer applies private key; user unblinds the result. Output is a valid signature on the original message — signer never sees the message. Used in anonymous e-cash, voting systems.  
- **Group Signatures**: any group member can sign; the verifier knows it came from *a* group member but cannot identify which one; a designated group manager can revoke anonymity if needed.  
- **Secure Multiparty Computation (MPC)**: multiple parties compute a function on their private inputs without revealing those inputs. Inputs split via secret sharing; computation expressed as a circuit (AND, XOR gates); result reconstructed from shares.  
- **Fully Homomorphic Encryption (FHE)**: compute on encrypted data without decrypting. Addition → XOR, multiplication → AND on ciphertext. Problem: each operation adds noise; too much noise breaks decryption. Bootstrapping resets noise by re-encrypting the ciphertext homomorphically.  
- **Perfect Forward Secrecy (PFS)**: session keys derived freshly for each session; long-term keys used only for authentication; past session keys cannot be recovered even if long-term key is later compromised. TLS achieves PFS with ephemeral Diffie-Hellman.  
- **Key rotation**: limit the amount of data encrypted under any single key; periodic rotation reduces exposure.  
- **Integrity protection**: encryption alone does not prevent bit-flipping attacks (malleable ciphertexts). Add a MAC or use authenticated encryption (e.g., AES-GCM).  
- **Nonce reuse**: reusing a nonce in stream cipher or authenticated encryption mode breaks confidentiality and/or integrity.  
   
    
   
   **Key Taxonomy**  
   
    
   
     
   
    
   
   | | |  
   
    
   
     
   
    
   
   |-|-|  
   
    
   
     
   
    
   
   | **Problem** |    **Tool** |  
   
    
   
     
   
    
   
   | Single point of failure for secrets | Secret sharing |  
   
    
   
     
   
    
   
   | Sign without seeing the message | Blind signatures |  
   
    
   
     
   
    
   
   | Sign anonymously as a group member | Group signatures |  
   
    
   
     
   
    
   
   | Compute on private data jointly | MPC |  
   
    
   
     
   
    
   
   | Compute on encrypted data | FHE |  
   
    
   
     
   
    
   
   | Past sessions safe if key compromised | Perfect forward secrecy |  
   
    
   
     
   
    
   
   | Encryption without integrity | Add MAC / use AEAD |  
   
    
   
     
   
    
   
      
   
    
   
   **Questions**  
1. Secret sharing requires k out of n shares. Why does holding k-1 shares reveal *nothing at all* about the secret?  
2. Why would a digital cash system need blind signatures? What is the property being protected?  
3. If two parties want to compute "who earns more" without revealing their salaries, how does MPC help?  
4. You encrypt 1 TB of data under a single key. The key is later compromised. How does key rotation limit the damage?  
5. A system uses AES-CBC for encryption but no MAC. Describe a bit-flipping attack. How does authenticated encryption (AES-GCM) prevent it?  
6. What is the difference between perfect forward secrecy and simply using a strong encryption algorithm?  
7. Why is nonce reuse in AES-GCM catastrophic in ways that nonce reuse in AES-CBC is not?  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAANElEQVR4nO3OQQmAABRAsScYxaA/i8kMIR7FCt5E2BJsmZmt2gMA4C+Otbqr8+sJAACvXQ85VAYUondDZQAAAABJRU5ErkJggg==)  
   
    
   
   **14. Cross-Cutting Themes (High Exam Probability)**  
   
    
   
     
   
    
   
   These themes appear across multiple modules. Expect exam questions that connect them.  
   
    
   
   **The Trust Ladder**  
   
    
   
     
   
    
   
   Infrastructure security and content security solve different problems. Be able to place each mechanism:  
   
    
   
     
   
    
   
   | | | |  
   
    
   
     
   
    
   
   |-|-|-|  
   
    
   
     
   
    
   
   | **Mechanism** |    **What it protects** |    **What it does NOT protect** |  
   
    
   
     
   
    
   
   | DNSSEC | Name → address mapping | Confidentiality; routing |  
   
    
   
     
   
    
   
   | BGPSEC + RPKI | Routing paths and origins | Names; content |  
   
    
   
     
   
    
   
   | PKI / Certificates | Identity (who owns a key) | Content; routing; names |  
   
    
   
     
   
    
   
   | Encryption (TLS, SSH, QUIC) | Content confidentiality and integrity | Names; routing; identity (on its own) |  
   
    
   
     
   
    
   
   | Anonymity (TOR) | Who is communicating | Content (need encryption too) |  
   
    
   
     
   
    
   
      
   
    
   
   **Common Crypto Failure Modes**  
   
    
   
     
   
    
   
   The exam will likely ask about why systems fail in practice:  
8. **Nonce reuse** — breaks stream cipher security, breaks AES-GCM authentication.  
9. **Weak randomness** — predictable keys (Debian bug), predictable seeds (timestamp-based).  
10. **Key misuse** — using the same key for encryption and integrity; using a signing key for decryption.  
11. **Trusting without verifying** — BGP peers, first-time SSH connections, DNS responses without DNSSEC.  
12. **Encryption without integrity** — malleable ciphertexts; add a MAC always.  
13. **No forward secrecy** — long-term key compromise exposes all past sessions.  
   
    
   
   **Questions (Cross-Cutting)**  
14. You are designing a messaging app. Rank these properties in order of importance: confidentiality, integrity, authentication, anonymity. Justify your ranking.  
15. "The Internet's security was added after the fact, not built in." Pick three protocols from this course that illustrate this point.  
16. A nation-state adversary controls Tier-1 ISP routing. Which security mechanisms in this course protect you? Which ones do not?  
17. An exam question says "the system uses AES with a 256-bit key." Is this system secure? What information is missing?  
18. Explain why "security through obscurity" fails. Use at least two examples from the course.  
   
    
   
   ![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAnEAAAACCAYAAAA3pIp+AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAM0lEQVR4nO3KsQ0AIRAEsUW6Tmj0+vjqvhMSYmKQ7GiCGd09k3wBAOAVf+2o4wYAwE1qAdY6Ay2/n6+dAAAAAElFTkSuQmCC)  
