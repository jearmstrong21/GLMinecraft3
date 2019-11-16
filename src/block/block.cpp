//
// Created by Jack Armstrong on 11/15/19.
//

#include "block.h"

namespace block {

    void renderFullBlock(gl::MeshData*md,BlockContext*ctx,client::QuadTextureDescr xmi,client::QuadTextureDescr xpl,client::QuadTextureDescr ymi,client::QuadTextureDescr ypl,client::QuadTextureDescr zmi,client::QuadTextureDescr zpl){
        client::QuadDescr qd;
        if(ctx->xmi==0){
            qd.xmi();
            qd.texture=xmi;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }
        if(ctx->xpl==0){
            qd.xpl();
            qd.texture=xpl;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }

        if(ctx->ymi==0){
            qd.ymi();
            qd.texture=ymi;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }
        if(ctx->ypl==0){
            qd.ypl();
            qd.texture=ypl;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }

        if(ctx->zmi==0){
            qd.zmi();
            qd.texture=zmi;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }
        if(ctx->zpl==0){
            qd.zpl();
            qd.texture=zpl;
            client::meshQuad(md,qd,ctx->pos.x,ctx->pos.y,ctx->pos.z);
        }
    }

}