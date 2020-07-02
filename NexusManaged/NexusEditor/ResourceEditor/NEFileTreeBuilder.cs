using System;
using System.Windows.Forms;

using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 添加到TreeNode中的数据
    /// </summary>
    struct NEFileTag
    {
        public string pkgName;
        public string path;
    }

    /// <summary>
    /// 递归查询所有子目录
    /// </summary>
    class NEFolderTreeWalker : NFileQuery
    {
        private TreeNode m_parentNode;
        private string m_pkgName;
        private string m_path;

        public NEFolderTreeWalker(TreeNode parentNode, string pkgName, string path)
        {
            m_parentNode = parentNode;
            m_pkgName = pkgName;
            m_path = path;
        }

        public override void OnFileEntity(ref NFileEntity file)
        {
            if (file.type == EFileEntityType.Folder)
            {
                TreeNode folderNode = m_parentNode.Nodes.Add(file.FileName);
                folderNode.SelectedImageIndex = 1;
                folderNode.ImageIndex = 0;

                //-- set tag to tree node
                NEFileTag tag;
                tag.pkgName = m_pkgName;
                tag.path = file.path;
                folderNode.Tag = tag;

                //-- query folder recursivly
                NFileSystem fileSys = Program.engine.FileSystem;
                NEFolderTreeWalker fq = new NEFolderTreeWalker(folderNode, m_pkgName, file.path);
                fileSys.QueryPackage(m_pkgName, file.path, fq);
            }
        }
    }

    /// <summary>
    /// 查询文件系统中某个Package中的所有目录
    /// </summary>
    class NEPkgTreeWalker : NFileQuery
    {
        private TreeNode m_pkgNode;
        private string m_pkgName;

        public NEPkgTreeWalker(TreeNode pkgNode, string pkgName)
        {
            m_pkgNode = pkgNode;
            m_pkgName = pkgName;
        }

        public override void OnFileEntity(ref NFileEntity file) 
        {
            if (file.type == EFileEntityType.Folder)
            {
                TreeNode folderNode = m_pkgNode.Nodes.Add(file.FileName);
                folderNode.SelectedImageIndex = 1;
                folderNode.ImageIndex = 0;

                //-- set tag
                NEFileTag tag;
                tag.pkgName = m_pkgName;
                tag.path = file.path;
                folderNode.Tag = tag;

                //-- query the folder
                NFileSystem fileSys = Program.engine.FileSystem;
                NEFolderTreeWalker fq = new NEFolderTreeWalker(folderNode, m_pkgName, file.path);
                fileSys.QueryPackage(m_pkgName, file.path, fq);
            }
        }
    }

    /// <summary>
    /// 查询文件系统中的的每个package
    /// </summary>
    class NEFileTreeBuilder
    {
        private TreeView m_treeCtrl;

        public NEFileTreeBuilder(TreeView treeCtrl)
        {
            m_treeCtrl = treeCtrl;
        }

        public void BuildTree()
        {
            m_treeCtrl.BeginUpdate();
            m_treeCtrl.Nodes.Clear();
            
            TreeNode rootNode = m_treeCtrl.Nodes.Add("FileRoot");
            rootNode.ImageIndex = 2;
            rootNode.SelectedImageIndex = 2;

            NFileSystem fileSys = Program.engine.FileSystem;

            int numPkg = fileSys.GetNumPackage();
            for (int i = 0; i < numPkg; i++)
            {
                string pkgName = fileSys.GetPackageName(i);
                TreeNode pkgNode = rootNode.Nodes.Add(pkgName);
                pkgNode.ImageIndex = 3;
                pkgNode.SelectedImageIndex = 3;

				NEFileTag tag;
				tag.pkgName = pkgName;
				tag.path = "";
				pkgNode.Tag = tag;

                NEPkgTreeWalker fq = new NEPkgTreeWalker(pkgNode, pkgName);
                fileSys.QueryPackage(pkgName, "/", fq);

                pkgNode.Expand();
            }

            rootNode.Expand();
            m_treeCtrl.EndUpdate();
        }
    }
}
