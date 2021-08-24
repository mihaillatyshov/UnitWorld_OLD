/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
#include <assert.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "util.h"
#include "math_3d.h"
#include "texture.h"
#include "Core/Texture.h"

using namespace std;

struct SceneAnimNode {
    std::string mName;
    SceneAnimNode* mParent;
    std::vector<SceneAnimNode*> mChildren;

    //! most recently calculated local transform
    aiMatrix4x4 mLocalTransform;

    //! same, but in world space
    aiMatrix4x4 mGlobalTransform;

    //!  index in the current animation's channel array. -1 if not animated.
    int mChannelIndex;

    //! Default construction
    SceneAnimNode()
    : mName()
    , mParent(nullptr)
    , mChildren()
    , mLocalTransform()
    , mGlobalTransform()
    , mChannelIndex(-1) {
        // empty
    }

    //! Construction from a given name
    SceneAnimNode( const std::string& pName)
    : mName( pName)
    , mParent(nullptr)
    , mChildren()
    , mLocalTransform()
    , mGlobalTransform()
    , mChannelIndex(-1) {
        // empty
    }

    //! Destruct all children recursively
    ~SceneAnimNode() {
        for (std::vector<SceneAnimNode*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
            delete *it;
        }
    }
};

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const string& Filename);

    void Render();
	
    uint NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds, vector<Matrix4f>& Transforms, size_t AnimID = 0);


	inline bool HasAnimations() const { return m_pScene->HasAnimations(); }
	inline size_t GetAnimationsCount() const { return m_pScene->mNumAnimations; }
	const aiAnimation* GetAnimation(int id) { return m_pScene->mAnimations[id]; }

	const aiScene* GetScene() const { return m_pScene; }

	aiNode* MeshNode;

	//void SetAnimIndex(size_t pAnimIndex);
	//
	//
	//void CalculateGlobalTransform(SceneAnimNode* pInternalNode);
	//void UpdateTransforms(SceneAnimNode* pNode, const std::vector<aiMatrix4x4>& pTransforms);
	//void Evaluate(const aiAnimation* mAnim, double pTime);
    //
	//typedef std::map<const char*, const aiNode*> BoneMap;
    //BoneMap mBoneNodesByName;
	//
	//SceneAnimNode* mRootNode;
    //int mCurrentAnimIndex;
	//
	//std::vector<aiMatrix4x4> mTransforms;
	//std::vector<std::tuple<unsigned int, unsigned int, unsigned int> > mLastPositions;
    //double mLastTime;
    
private:
    #define NUM_BONES_PER_VEREX 4

    struct BoneInfo
    {
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;        

        BoneInfo()
        {
            BoneOffset.SetZero();
            FinalTransformation.SetZero();            
        }
    };
    
    struct VertexBoneData
    {        
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(uint BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform, size_t AnimID = 0);
    bool InitFromScene(const aiScene* pScene, const string& Filename);
    void InitMesh(uint MeshIndex,
                  const aiMesh* paiMesh,
                  vector<Vector3f>& Positions,
                  vector<Vector3f>& Normals,
                  vector<Vector2f>& TexCoords,
                  vector<VertexBoneData>& Bones,
                  vector<unsigned int>& Indices);
    void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
    bool InitMaterials(const aiScene* pScene, const string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    vector<MeshEntry> m_Entries;
    vector<Texture*> m_Textures;
     
    map<string,uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    Matrix4f m_GlobalInverseTransform;
    
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;


};


#endif	/* MESH_H */

