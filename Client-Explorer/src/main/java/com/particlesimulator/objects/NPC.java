package com.particlesimulator.objects;

import com.particlesimulator.Utils.Position;

public class NPC extends Entity {
    private String uid;
    private ObjectManager objectManager;
    public NPC(String id, ObjectManager manager, Position pos) {
        super(pos, false);
        this.uid = id;
        this.objectManager = manager;
    }
}
