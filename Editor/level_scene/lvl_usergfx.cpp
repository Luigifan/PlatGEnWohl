/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "lvlscene.h"
#include "../leveledit.h"


//Search and load custom User's files
void LvlScene::loadUserData(LevelData FileData, QProgressDialog &progress)
{
    int i, total=0;

    UserBGs uBG;
    UserBlocks uBlock;
    UserBGOs uBGO;

    bool loaded1, loaded2;
    QString uLVLDs = FileData.path + "/" + FileData.filename + "/";
    QString uLVLD = FileData.path + "/" + FileData.filename;
    QString uLVLs = FileData.path + "/";

    //Load Backgrounds
    for(i=0; i<pConfigs->main_bg.size(); i++) //Add user images
        {
        if(!progress.wasCanceled())
            progress.setLabelText("Search User Backgrounds "+QString::number(i)+"/"+QString::number(pConfigs->main_bg.size()));

            loaded1 = false;
            loaded2 = false;

            //check for first image
            if((QFile::exists(uLVLD) ) &&
                  (QFile::exists(uLVLDs + pConfigs->main_bg[i].image_n)) )
            {
                uBG.image = QPixmap( uLVLDs + pConfigs->main_bg[i].image_n );
                uBG.id = pConfigs->main_bg[i].id;
                loaded1 = true;
            }
            else
            if(QFile::exists(uLVLs + pConfigs->main_bg[i].image_n) )
            {
                uBG.image = QPixmap( uLVLs + pConfigs->main_bg[i].image_n );
                uBG.id = pConfigs->main_bg[i].id;
                loaded1 = true;
            }

            if((loaded1)&&(pConfigs->main_bg[i].animated) )
            {
                uBG.image=uBG.image.copy(0, 0, uBG.image.width(), (int)round(uBG.image.height()/pConfigs->main_bg[i].frames));
            }

            //check for second image
            if(pConfigs->main_bg[i].type == 1)
            {
                if((QFile::exists(uLVLD) ) &&
                      (QFile::exists(uLVLDs + pConfigs->main_bg[i].second_image_n )) )
                {
                    uBG.second_image = QPixmap( uLVLDs + pConfigs->main_bg[i].second_image_n );
                    uBG.id = pConfigs->main_bg[i].id;
                    loaded2 = true;
                }
                else
                if(QFile::exists(uLVLs + pConfigs->main_bg[i].second_image_n) )
                {
                    uBG.second_image = QPixmap( uLVLs + pConfigs->main_bg[i].second_image_n );
                    uBG.id = pConfigs->main_bg[i].id;
                    loaded2 = true;
                }
            }
            if((loaded1)&&(!loaded2)) uBG.q = 0;
            if((!loaded1)&&(loaded2)) uBG.q = 1;
            if((loaded1)&&(loaded2)) uBG.q = 2;

            //If user images found and loaded
            if( (loaded1) || (loaded2) )
                uBGs.push_back(uBG);

        total++;
        if(!progress.wasCanceled())
            progress.setValue(progress.value()+1);
        else return;
        }


    //Load Blocks
    for(i=0; i<pConfigs->main_block.size(); i++) //Add user images
    {

        if(!progress.wasCanceled())
            progress.setLabelText("Search User Blocks "+QString::number(i)+"/"+QString::number(pConfigs->main_block.size()));

            if((QFile::exists(uLVLD) ) &&
                  (QFile::exists(uLVLDs + pConfigs->main_block[i].image_n)) )
            {
                if(QFile::exists(uLVLDs + pConfigs->main_block[i].mask_n))
                    uBlock.mask = QBitmap(uLVLDs + pConfigs->main_block[i].mask_n );
                else
                    uBlock.mask = pConfigs->main_block[i].mask;

                uBlock.image = QPixmap(uLVLDs + pConfigs->main_block[i].image_n );

                if((uBlock.image.height()!=uBlock.mask.height())||(uBlock.image.width()!=uBlock.mask.width()))
                    uBlock.mask = uBlock.mask.copy(0,0,uBlock.image.width(),uBlock.image.height());
                uBlock.image.setMask(uBlock.mask);
                uBlock.id = pConfigs->main_block[i].id;
                uBlocks.push_back(uBlock);

                //Apply index;
                if(uBlock.id < (unsigned int)index_blocks.size())
                {
                    index_blocks[uBlock.id].type = 1;
                    index_blocks[uBlock.id].i = (uBlocks.size()-1);
                }
            }
            else
            if(QFile::exists(uLVLs + pConfigs->main_block[i].image_n) )
            {
                if(QFile::exists(uLVLs + pConfigs->main_block[i].mask_n))
                    uBlock.mask = QBitmap(uLVLs + pConfigs->main_block[i].mask_n );
                else
                    uBlock.mask = pConfigs->main_block[i].mask;

                uBlock.image = QPixmap(uLVLs + pConfigs->main_block[i].image_n );

                if((uBlock.image.height()!=uBlock.mask.height())||(uBlock.image.width()!=uBlock.mask.width()))
                    uBlock.mask = uBlock.mask.copy(0,0,uBlock.image.width(),uBlock.image.height());

                uBlock.image.setMask(uBlock.mask);
                uBlock.id = pConfigs->main_block[i].id;
                uBlocks.push_back(uBlock);

                //Apply index;
                if(uBlock.id < (unsigned int)index_blocks.size())
                {
                    index_blocks[uBlock.id].type = 1;
                    index_blocks[uBlock.id].i = (uBlocks.size()-1);
                }
            }

    if(!progress.wasCanceled())
        progress.setValue(progress.value()+1);
    else return;
    }

    //Load BGO
    for(i=0; i<pConfigs->main_bgo.size(); i++) //Add user images
    {
        if(!progress.wasCanceled())
            progress.setLabelText("Search User BGOs "+QString::number(i)+"/"+QString::number(pConfigs->main_bgo.size()));

            if((QFile::exists(uLVLD) ) &&
                  (QFile::exists(uLVLDs + pConfigs->main_bgo[i].image_n)) )
            {
                if(QFile::exists(uLVLDs + pConfigs->main_bgo[i].mask_n))
                    uBGO.mask = QBitmap(uLVLDs + pConfigs->main_bgo[i].mask_n );
                else
                    uBGO.mask = pConfigs->main_bgo[i].mask;

                uBGO.image = QPixmap(uLVLDs + pConfigs->main_bgo[i].image_n );

                if((uBGO.image.height()!=uBGO.mask.height())||(uBGO.image.width()!=uBGO.mask.width()))
                    uBGO.mask = uBGO.mask.copy(0,0,uBGO.image.width(),uBGO.image.height());
                uBGO.image.setMask(uBGO.mask);
                uBGO.id = pConfigs->main_bgo[i].id;
                uBGOs.push_back(uBGO);
            }
            else
            if(QFile::exists(uLVLs + pConfigs->main_bgo[i].image_n) )
            {
                if(QFile::exists(uLVLs + pConfigs->main_bgo[i].mask_n))
                    uBGO.mask = QBitmap(uLVLs + pConfigs->main_bgo[i].mask_n );
                else
                    uBGO.mask = pConfigs->main_bgo[i].mask;

                uBGO.image = QPixmap(uLVLs + pConfigs->main_bgo[i].image_n );

                if((uBGO.image.height()!=uBGO.mask.height())||(uBGO.image.width()!=uBGO.mask.width()))
                    uBGO.mask = uBGO.mask.copy(0,0,uBGO.image.width(),uBGO.image.height());

                uBGO.image.setMask(uBGO.mask);
                uBGO.id = pConfigs->main_bgo[i].id;
                uBGOs.push_back(uBGO);
            }
    if(!progress.wasCanceled())
        progress.setValue(progress.value()+1);
    else return;
    }

}