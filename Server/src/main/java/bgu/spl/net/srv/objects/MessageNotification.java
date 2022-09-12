package bgu.spl.net.srv.objects;

import bgu.spl.net.srv.bidi.Message;

import java.io.Serializable;

public class MessageNotification implements Serializable {

    public short OPCODE;
    byte[] optional;
    byte PMorPublic;


    public MessageNotification(byte PMorPublic, byte[] optional){
        OPCODE = 9;
        this.optional = optional;
        this.PMorPublic = PMorPublic;
    }

    public short getOpcode() {
        return OPCODE;
    }

    public byte getPMorPublic() {
        return PMorPublic;
    }

    public byte[] getOptional() {
        return optional;
    }
}
