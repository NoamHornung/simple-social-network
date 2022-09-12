package bgu.spl.net.impl.BGSServer;

import bgu.spl.net.srv.BidiMessagingProtocolImpl;
import bgu.spl.net.srv.MessageEncoderDecoderImpl;
import bgu.spl.net.srv.Server;

public class ReactorMain {
    public static void main(String[] args) {

        Server.reactor(
                Integer.parseInt(args[1]),
                Integer.parseInt(args[0]),
                () -> new BidiMessagingProtocolImpl<>(), //protocol factory
                () -> new MessageEncoderDecoderImpl<>()//message encoder decoder factory
        ).serve();
    }
}
