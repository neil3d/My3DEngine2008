using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Nexus.GameFramework;
using Nexus.GUI;
using NexusEngine;
using NexusEngineExtension;

namespace NexusGameClient
{
	class MiniMapFrame: IGameFrame
	{
		//小地图的绘制器
		private Nexus.GUI.Widgets.UIShapedImageBox miniMap;

		//坐标显示器
		private Nexus.GUI.Widgets.UILabel coordLabel;

		//小地图的大小
		private Vector2 miniMapSize;

		//整个场景的大小
		private Vector2 sceneSize;

		//小地图的显示范围，默认是角色周围128*128像素的范围,实际为了计算方便，保存一半的值
		private Vector2 minimapRange;

		//小地图的缩放级别
		private float[]	minimapZoomLevel;

		//小地图的当前缩放级别
		private int currentZoomLevel;

		//小地图的显示比例
		private Vector2 minimapRatio;

		//角色在小地图中的位置
		private Vector2 playerLocation2D;

		//图标纹理
		private NResourceTextureAtlas Icons;

		//表示当前角色的图标
		private Rect playerIcon;

		//表示周围角色的图标
		private Rect aroundRoleIcon;

		//代表指北针的图标
		private Rect northIcon;

		//小地图的旋转值
		private Matrix miniMapRotation;

		//周围角色列表
		private List<RoleAround> roleAround;

		public MiniMapFrame()
		{
			currentZoomLevel = 2;
			minimapZoomLevel = new float[5] { 160.0f, 128.0f, 96.0f, 64.0f, 32.0f };
			sceneSize = new Vector2();
			minimapRange = new Vector2(minimapZoomLevel[currentZoomLevel], minimapZoomLevel[currentZoomLevel]);
			minimapRatio = new Vector2(1.0f, 1.0f);
			roleAround = new List<RoleAround>();
		}

		public void Destroyed()
        {
        }

        public void DrawFrame(GameTime gameTime)
        {
        }

        public bool LoadFrame()
        {
			WindowCollection windowCollection = GameHelper.LoadUIForm(GUISystem.Instance.RootWindow, "content", "/ui/form/MiniMap.nui");
			if (windowCollection != null)
			{
				miniMap = (Nexus.GUI.Widgets.UIShapedImageBox)GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UIShapedImageBox1", true);
				coordLabel = (Nexus.GUI.Widgets.UILabel)GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UILabel1",true);
				if(miniMap!=null)
				{
					miniMap.BackgroundRenderComp.ImageLocation = new NResourceLoc("content", "/minimap/minimap.dds");
					miniMapSize = miniMap.BackgroundRenderComp.Texture.GetOriginalSize();
					Vector2 center = new Vector2(miniMapSize.x * 0.5f, miniMapSize.y * 0.5f);
					miniMap.BackgroundRenderComp.TextureRect = new Rect(center.x - minimapRange.x, center.y - minimapRange.y,
						center.x + minimapRange.x, center.y + minimapRange.y);
					Vector3 scale = GameEngine.EngineInstance.CurrentLevel.NavigationMapScale;
					sceneSize.x = GameEngine.EngineInstance.CurrentLevel.NavigationMapWidth * scale.x;
					sceneSize.y = GameEngine.EngineInstance.CurrentLevel.NavigationMapHeight * scale.z;
					minimapRatio.x = minimapRange.x * 2 / miniMap.Width.offest;
					minimapRatio.y = minimapRange.y * 2 / miniMap.Height.offest;

					Icons = NResourceManager.Instance.LoadTextureAtlas(new NResourceLoc("content", "/ui/texAtlas/content1.txa"), EResourceIOMode.Auto, EResourceIOPriority.Normal);
					if (Icons == null) return false;

					{
						NTexAtlasItem item = Icons.GetItem("minimap_6");
						playerIcon = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);
					}

					{
						NTexAtlasItem item = Icons.GetItem("minimap-5");
						northIcon = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);
					}

					{
						NTexAtlasItem item = Icons.GetItem("minimap-5");
						aroundRoleIcon = new Rect(item.UVStart.x, item.UVStart.y, item.UVEnd.x, item.UVEnd.y);
					}
					//------------------------------------------------------
					miniMap.BackgroundRenderComp.OnRendering += this.OnDraw;

					Window win = GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UIButton2", true);
					if (win != null) win.MouseClicked += this.ZoomIn;

					win = GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UIButton3", true);
					if (win != null) win.MouseClicked += this.ZoomOut;

					win = GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UIButton4", true);
					if (win != null) win.MouseClicked += this.OnMinsize;

					win = GUISystem.Instance.RootWindow.FindChild("root/UIStaticImage1/UIButton5", true);
					if (win != null) win.MouseClicked += this.OnShowWorldMap;


					// 添加测试用角色
					RoleAround a = new RoleAround(1, RoleStandpoint.RoleFriendly); roleAround.Add(a);
					RoleAround b = new RoleAround(2, RoleStandpoint.RoleNeutrally); roleAround.Add(b);
					RoleAround c = new RoleAround(3, RoleStandpoint.RoleHostile); roleAround.Add(c);
					a.Position = new Vector2(500, 400);
					b.Position = new Vector2(450, 500);
					c.Position = new Vector2(550, 450);
				}
			}

            return windowCollection!=null;
        }

		public void NotifyNetConnected(NetMessageEvent e)
		{
		}

        public void NotifyNetMessageReceived(MessageReceivedEvent e)
        {
			if (e.messageId == (UInt16)NetMovement.SC_Move)
			{
				//角色移动，更新小地图的显示范围，如果角色移动到了地图的边缘，则不改变对应边缘的小地图显示
				//OnRecvCharacterMove(e);
			}
			else if (e.messageId == (UInt16)NetMovement.SC_PlayerEnterAoi)
			{
				//其他角色进入视野
				//OnRecvPlayerEnterAoi(e);
			}
			else if (e.messageId == (UInt16)NetMovement.SC_CreatureEnterAoi)
			{
				//其他生物进入视野
				//OnRecvCreatureEnterAoi(e);
			}
			else if (e.messageId == (UInt16)NetMovement.SC_GameObjectEnterAoi)
			{
				//其他游戏对象进入AOI
			}
			else if (e.messageId == (UInt16)NetMovement.SC_ObjectLeaveAoi)
			{
				// 游戏对象离开AOI
			}
			else if (e.messageId == (UInt16)NetLoading.SC_GetInitStat)
			{
				//进入地图，初始化小地图？
			}
        }

		public void Update(float deltaTime, NViewport view)
        {
			//计算角色在小地图中的坐标
			Vector3 pos = GameEngine.EngineInstance.LocalPlayer.Location;
			playerLocation2D = WorldToMiniMap(new Vector2(pos.x,pos.z));

			Vector2 mapCenter = playerLocation2D;
			if (mapCenter.x < minimapRange.x)
			{
				mapCenter.x = minimapRange.x;
			}
			if (mapCenter.x > miniMapSize.x - minimapRange.x)
			{
				mapCenter.x = miniMapSize.x - minimapRange.x;
			}

			if (mapCenter.y < minimapRange.y)
			{
				mapCenter.y = minimapRange.y;
			}
			if (mapCenter.y > miniMapSize.y - minimapRange.y)
			{
				mapCenter.y = miniMapSize.y - minimapRange.y;
			}

			//改变小地图的显示范围
			Rect rc = new Rect(mapCenter.x - minimapRange.x, mapCenter.y - minimapRange.y,
				mapCenter.x + minimapRange.x, mapCenter.y + minimapRange.y);
			miniMap.BackgroundRenderComp.TextureRect = rc;

			//旋转小地图
			miniMap.BackgroundRenderComp.Rotation = GameEngine.EngineInstance.LocalPlayer.GameplayCamera.Yaw + (float)Math.PI;
			Rect mmrc = miniMap.BackgroundRenderComp.TargetRect;
			Matrix trans=Matrix.Translation(mmrc.left+mmrc.Width*0.5f, mmrc.top+mmrc.Height*0.5f, 0);
			Matrix invTrans = trans; 
			invTrans.Invert();
			miniMapRotation = Matrix.RotationAxis(new Vector3(0, 0, 1), miniMap.BackgroundRenderComp.Rotation);
			miniMapRotation = invTrans * miniMapRotation * trans;
			
			//显示当前坐标
			if (coordLabel != null)
			{
				int x = (int)pos.x;
				int y = (int)pos.z;
				String text = x.ToString();
				text += ",";
				text += y.ToString();
				coordLabel.TextRenderComp.Text = text;
			}
        }

		public Vector2	WorldToMiniMap(Vector2 pos)
		{
			Vector2 ret = new Vector2();
			ret.x = (pos.x / sceneSize.x + 0.5f) * miniMapSize.x;
			ret.y = (1.0f-(pos.y / sceneSize.y + 0.5f)) * miniMapSize.y;
			return ret;
		}

		public Vector2 MiniMapToScreen(Vector2 pos)
		{
			Vector2 ret = new Vector2();
			Rect mmrc = miniMap.BackgroundRenderComp.TargetRect;
			Rect textureRect = miniMap.BackgroundRenderComp.TextureRect;
			ret.x = mmrc.left + (pos.x - textureRect.left) / minimapRatio.x;
			ret.y = mmrc.top + (pos.y - textureRect.top) / minimapRatio.y;
			//应用小地图的旋转
			Vector4 rst = Vector2.Transform(ret, miniMapRotation);
			ret.x = rst.x;
			ret.y=rst.y;
			return ret;
		}

		public Vector2 WorldToScreen(Vector2 pos)
		{
			Vector2 ret = new Vector2();
			ret.x = (pos.x / sceneSize.x + 0.5f) * miniMapSize.x;
			ret.y = (1.0f-(pos.y / sceneSize.y + 0.5f))* miniMapSize.y;
			Rect mmrc = miniMap.BackgroundRenderComp.TargetRect;
			Rect textureRect = miniMap.BackgroundRenderComp.TextureRect;
			ret.x = mmrc.left + (ret.x - textureRect.left) / minimapRatio.x;
			ret.y = mmrc.top + (ret.y - textureRect.top) / minimapRatio.y;
			//应用小地图的旋转
			Vector4 rst = Vector2.Transform(ret, miniMapRotation);
			ret.x = rst.x;
			ret.y = rst.y;
			return ret;
		}

		public Vector2 MiniMapToWorld(Vector2 pos)
		{
			Vector2 ret = new Vector2();
			ret.x = (pos.x / miniMapSize.x - 0.5f) * sceneSize.x;
			ret.y = (0.5f - pos.y / miniMapSize.y) * sceneSize.y;
			return ret;
		}

		public Vector2 ScreenToMiniMap(Vector2 pos)
		{
			Vector2 ret = new Vector2();
			Rect mmrc = miniMap.BackgroundRenderComp.TargetRect;
			Rect textureRect = miniMap.BackgroundRenderComp.TextureRect;
			ret.x = (pos.x - mmrc.left) * minimapRatio.x + textureRect.left;
			ret.y = (pos.x - mmrc.top) * minimapRatio.y + textureRect.top;
			//应用逆矩阵
			Matrix inv = miniMapRotation;
			inv.Invert();
			Vector4 rst = Vector2.Transform(ret, inv);
			ret.x = rst.x;
			ret.y = rst.y;
			return ret;
		}

		public Vector2 ScreenToWorld(Vector2 pos)
		{
			return MiniMapToWorld(ScreenToMiniMap(pos));
		}

		//指定坐标是否在小地图显示范围内
		public bool IsVisible(Vector2 pos,float radius)
		{
			Rect rc = miniMap.BackgroundRenderComp.TextureRect;
			if (!rc.Contains(pos)) return false;
			Vector2 ori = new Vector2(rc.left + rc.Width * 0.5f, rc.top + rc.Height * 0.5f);
			ori -= pos;
			if(ori.Length()>=minimapRange.x-radius)
			{
				return false;
			}
			return true;
		}

		//改变要显示的小地图
		public void ChangeMap(NResourceLoc resLoc)
		{
			try	
			{
				miniMap.BackgroundRenderComp.ImageLocation = resLoc;
				miniMapSize = miniMap.BackgroundRenderComp.Texture.GetOriginalSize();
				Vector2 center = new Vector2(miniMapSize.x * 0.5f, miniMapSize.y * 0.5f);
				miniMap.BackgroundRenderComp.TextureRect = new Rect(center.x - minimapRange.x, center.y - minimapRange.y,
					center.x + minimapRange.x, center.y + minimapRange.y);
				Vector3 scale = GameEngine.EngineInstance.CurrentLevel.NavigationMapScale;
				sceneSize.x = GameEngine.EngineInstance.CurrentLevel.NavigationMapWidth * scale.x;
				sceneSize.y = GameEngine.EngineInstance.CurrentLevel.NavigationMapHeight * scale.z;
				minimapRatio.x = minimapRange.x * 2 / miniMap.Width.offest;
				minimapRatio.y = minimapRange.y * 2 / miniMap.Height.offest;
			}
			catch(Exception)
			{
				//不需要处理
			}
			
		}

		//改变小地图的显示范围,小地图最多有5级缩放,每次提高或者降低一级.参数：true=放大,false=缩小
		public void	Zoom(bool zoomIn)
		{
			if(zoomIn)
			{
				++currentZoomLevel;
				if (currentZoomLevel > 4) currentZoomLevel = 4;
			}
			else
			{
				--currentZoomLevel;
				if (currentZoomLevel < 0) currentZoomLevel = 0;
			}
			minimapRange = new Vector2(minimapZoomLevel[currentZoomLevel], minimapZoomLevel[currentZoomLevel]);

			Vector2 center = new Vector2(miniMapSize.x * 0.5f, miniMapSize.y * 0.5f);
			miniMap.BackgroundRenderComp.TextureRect = new Rect(center.x - minimapRange.x, center.y - minimapRange.y,
				center.x + minimapRange.x, center.y + minimapRange.y);
			Vector3 scale = GameEngine.EngineInstance.CurrentLevel.NavigationMapScale;
			sceneSize.x = GameEngine.EngineInstance.CurrentLevel.NavigationMapWidth * scale.x;
			sceneSize.y = GameEngine.EngineInstance.CurrentLevel.NavigationMapHeight * scale.z;
			minimapRatio.x = minimapRange.x * 2 / miniMap.Width.offest;
			minimapRatio.y = minimapRange.y * 2 / miniMap.Height.offest;
		}

		//将指定的纹理旋转后绘制到小地图的目标区域，注意，目标区域和旋转中心使用小地图坐标系
		private void DrawImageInMap(NResourceTexture2D texture, Rect sourceRect, Rect targetRect, float rotation,Vector2 rotCenter,Color4f color)
		{
			Vector2 srCenter = new Vector2(targetRect.left + targetRect.Width * 0.5f, targetRect.top + targetRect.Height * 0.5f);
			srCenter = MiniMapToScreen(srCenter);
			rotCenter = MiniMapToScreen(rotCenter);
			rotation += miniMap.BackgroundRenderComp.Rotation - (float)Math.PI;
			float width = targetRect.Width ;
			float height = targetRect.Height ;
			GUISystem.Instance.Renderer.DrawTexture(texture, sourceRect, new Rect(srCenter.x - width, srCenter.y - height, srCenter.x + width, srCenter.y + height), rotation, rotCenter, color.ToArgb());
		}
		
		//绘制代表玩家的箭头
		private void	DrawPlayerPosition()
		{
			Rect rc = new Rect(playerLocation2D.x - 4, playerLocation2D.y - 4, playerLocation2D.x + 4, playerLocation2D.y + 4);
			float rot = GameEngine.EngineInstance.LocalPlayer.Rotation.y;
			DrawImageInMap(Icons,playerIcon, rc, rot, playerLocation2D, new Color4f(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//绘制指北针,默认向上为北
		private void	DrawNorthPointer()
		{
			Rect mmrc = miniMap.BackgroundRenderComp.TargetRect;
			float cx = mmrc.left + mmrc.Width * 0.5f;
			float cy = mmrc.top + mmrc.Height * 0.5f;
			//计算初始条件下指北针在屏幕上的位置
			Rect rc = mmrc;
			Vector2 rotCenter = new Vector2(cx, cy);
			//绘制指北针，包含小地图的旋转
			GUISystem.Instance.Renderer.DrawTexture(Icons,northIcon, rc, miniMap.BackgroundRenderComp.Rotation+(float)Math.PI, rotCenter, new Color4f(1.0f, 1.0f, 1.0f, 1.0f).ToArgb());
		}

		//绘制周围的角色信息
		private void DrawAroundRoles()
		{
			foreach (RoleAround role in roleAround)
			{
				if (!IsVisible(role.Position,3)) continue;
				Rect rc = new Rect(role.Position.x - 3, role.Position.y - 3, role.Position.x + 3, role.Position.y + 3);
				float rot = GameEngine.EngineInstance.LocalPlayer.Rotation.y;
				DrawImageInMap(Icons,aroundRoleIcon, rc, rot, role.Position, role.Color);
			}
		}

		public void OnDraw(Object sender,GUIEventArgs args)
		{
			//绘制周围的角色信息
			DrawAroundRoles();

			//绘制指北针
			DrawNorthPointer();

			//绘制角色所在的位置
			DrawPlayerPosition();			
		}

		public void ZoomIn(Object sender, GUIEventArgs args)
		{
			Zoom(true);
		}

		public void ZoomOut(Object sender, GUIEventArgs args)
		{
			Zoom(false);
		}

		public void OnMinsize(Object sender, GUIEventArgs args)
		{
			//最小化
		}

		public void OnShowWorldMap(Object sender, GUIEventArgs args)
		{
			//显示大地图
		}
	}

	//周围角色的立场，不同的立场对应不同的色彩
	enum RoleStandpoint
	{
		RoleFriendly,	//友好的角色
		RoleNeutrally,	//中立的角色
		RoleHostile,	//敌对角色
	};

	//周围的角色
	class RoleAround
	{
		private Int64 m_ID;
		private RoleStandpoint m_Standpoint;
		private Vector2 m_Position;
		private Color4f m_Color;

		public RoleAround(Int64 id)
		{
			m_ID = id;
			Standpoint = RoleStandpoint.RoleNeutrally;
		}

		public RoleAround(Int64 id,RoleStandpoint standPoint)
		{
			m_ID = id;
			Standpoint = standPoint;
		}

		public Int64 ID
		{
			get { return m_ID; }
		}

		public RoleStandpoint Standpoint
		{
			get { return m_Standpoint; }
			set 
			{ 
				m_Standpoint = value;
				//不同立场的角色有不同的颜色，友好的为绿色，中立的是白色，敌对的是红色
				switch (m_Standpoint)
				{
					case RoleStandpoint.RoleFriendly:
						{
							m_Color= new Color4f(0.0f, 1.0f, 0.0f);
							break;
						}
					case RoleStandpoint.RoleNeutrally:
						{
							m_Color = new Color4f(1.0f, 1.0f, 1.0f);
							break;
						}
					case RoleStandpoint.RoleHostile:
						{
							m_Color = new Color4f(1.0f, 0, 0);
							break;
						}
				}
			}
		}

		//小地图空间的坐标数据
		public Vector2 Position
		{
			get { return m_Position; }
			set { m_Position=value;}
		}

		public Color4f Color
		{
			get
			{
				return m_Color;
			}
		}
	}
}
