/* 
 * Main entrypoint of the UpToDate application.
 * Andrei-Sebastian Lungu
 */
#include "../Networking/Peer/peer.h"

int main(void)
{
	// 1. Initiate the node. (default it will start the server).
	struct Peer peer = peer_constructor();
	
	// 2. Activate the 5 stations.
	peer.activate_stations(&peer);

	// 3. Open and run the node's server to accept pairings with other nodes.
	peer.run_server(&peer);

	// 4. Possibility of spreading.
	peer.plan_spread(&peer);
	
	// 5. Main thread infinite loop to prevent undefined behaviour.
	peer.main_thread_loop(&peer);
	return 0;
}
