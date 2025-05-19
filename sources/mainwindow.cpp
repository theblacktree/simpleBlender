#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "converttoshaderlight.h"

using namespace std;
using namespace glm;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_glwidget = nullptr;
    m_glwidget = new GLWidget(this, ui->openGLWidget_main);
    m_glwidget->resize(ui->openGLWidget_main->width(), ui->openGLWidget_main->height());

    m_colorDialog = new QColorDialog(this);
    m_colorDialog->setOption(QColorDialog::ShowAlphaChannel); // 可选：显示透明度选项
    m_colorDialog->setGeometry(20, 50, 100, 100);
    m_colorDialog->hide();
    // 创建并设置模型
   // m_model = new QStandardItemModel(this);
    m_model = new MyStandardItemModel(this);
    m_model->setHorizontalHeaderLabels({"实例管理"});
    m_itemObject = new QStandardItem("物体");
    m_model->appendRow(m_itemObject);
    m_itemLight = new QStandardItem("灯光");
    m_model->appendRow(m_itemLight);
    m_itemCamera = new QStandardItem("摄像机");
    m_model->appendRow(m_itemCamera);

    m_fileDialog = make_unique<QFileDialog>(new QFileDialog);
    connect(m_model, &MyStandardItemModel::itemTextChanged, this, &MainWindow::slotChangeItemName);
    ui->treeView_instance->setModel(m_model);
    ui->treeView_instance->expandAll();
    //connect(m_model, &QStandardItemModel::dataChanged,this, &MainWindow::slotChangeItemName);

    connect(ui->actionRenderImage, &QAction::triggered, this, &MainWindow::slotRenderImage);
    ui->treeView_instance->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui->btn_createObject, &QPushButton::clicked, this, &MainWindow::slotCreateObject);
    connect(ui->btn_basiccolor, &QPushButton::clicked, this, &MainWindow::slotPickColor);
    connect(ui->btn_createLight, &QPushButton::clicked, this, &MainWindow::slotCreateLight);
    connect(ui->btn_createCamera, &QPushButton::clicked, this, &MainWindow::slotCreateCamera);
    connect(ui->btn_lightColor, &QPushButton::clicked, this, &MainWindow::slotPickColor);
    connect(ui->btn_switchCamera, &QPushButton::clicked, this, &MainWindow::slotSwitchCamera);
    connect(ui->treeView_instance->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::slotSelectionChanged);
    //resize(1080,607);
    //这里先通过按钮打开文件选择器，然后选中文件打开时触发更新uv贴图的功能，这个功能交给glWidget设置物体的贴图
    connect(ui->btn_addUVTexture, & QPushButton::clicked, this,  &MainWindow::slotOpenUVTextureDialog);
    connect(m_fileDialog.get(), &QFileDialog::fileSelected, this,  &MainWindow::slotpassUVTextureInfo);
    //删除贴图
    connect(ui->btn_RemoveUVTexture, & QPushButton::clicked,this, &MainWindow::slotRemoveUVTexture);

    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;

    // 检查是否支持HDR和EXR,目前只支持hdr
    // std::vector<std::string> extensions = {"D:/hdr/snowy_field_4k.hdr", "D:/hdr/kloppenheim_06_puresky_4k.exr"};
    // for (const auto& ext : extensions) {
    //     bool supported = cv::haveImageReader(ext);
    //     std::cout << "Support for " << ext << ": " << (supported ? "Yes" : "No") << std::endl;
    // }
    Logger::GetInstance().run();
}

MainWindow::~MainWindow()
{
    Logger::GetInstance().stop();
    delete ui;
}

void MainWindow::slotPickColor()
{
    m_colorDialog->show();
    if (m_colorDialog->exec() == QDialog::Accepted)
    { // 如果用户点击了确定
        QColor selectedColor = m_colorDialog->currentColor();

        emit sigColorSelected(getSelectedVector(),selectedColor); // 发出信号
    }
}
vector<string> MainWindow::getSelectedVector()
{
    QModelIndexList selectedIndexes = ui->treeView_instance->selectionModel()->selectedIndexes();
    vector<string> strVec;
    if (!selectedIndexes.isEmpty())
    {
        for (const QModelIndex &index : selectedIndexes)
        {
            QStandardItem* item = m_model->itemFromIndex(index);
            if (item)
            {
                // 获取条目的名称
                string itemName = item->text().toStdString();
                strVec.emplace_back(itemName);
            }
        }
    }
    return strVec;
}

void MainWindow::slotCreateObject(bool checked)
{
    QString s;
    if (ui->comboBox_object->currentIndex() == 0)
    {
        cube* cu = new cube;
        s = "cube_" + QString::number(m_objectId);
        m_glwidget->addObject(cu, s);
    }
    else if (ui->comboBox_object->currentIndex() == 1)
    {
        UVSphere* uvsp = new UVSphere;
        s = "uvsphere_" + QString::number(m_objectId);
        m_glwidget->addObject(uvsp, s);
    }
    else if (ui->comboBox_object->currentIndex() == 2)
    {
        Torus* torus = new Torus;
        s = "torus_" + QString::number(m_objectId);
        m_glwidget->addObject(torus, s);
    }
    QStandardItem *itemObject;
    itemObject = new QStandardItem(s);
    m_itemObject->appendRow(itemObject);
    ++m_objectId;
}

void MainWindow::slotCreateLight()
{
    if (m_itemLight->rowCount() >= MAX_LIGHTS)
    {
        QMessageBox::warning(this, tr("Warning"), tr("灯光数量已满，请联系管理员增加容量！"));
        return ;
    }
    Light* light = nullptr;
    switch (ui->comboBox_light->currentIndex())
    {
        case 0://点光源
        {
            light = new PointLight( );
            break;
        }
        case 1://方向光源
        {
            light = new DirectionalLight();
            break;
        }
        case 2:
        {
            light = new SpotLight();
            break;
        }
        default:
        {
            cout<<"create fail"<<endl;
            break;
        }

    }
    m_glwidget->addLight(light, m_lightId);
    QStandardItem *itemLight;
    QString s = light->getLightTypeName();
    QString str = s +"_" + QString::number(m_lightId);
    itemLight = new QStandardItem(str);
    m_itemLight->appendRow(itemLight);
    ++m_lightId;
}

void MainWindow::slotCreateCamera()
{
    Camera* ca = new Camera();
    m_glwidget->addCamera(ca, m_cameraId);
    QStandardItem *itemCamera;
    QString s = "camera_" + QString::number(m_cameraId);
    itemCamera = new QStandardItem(s);
    m_itemCamera->appendRow(itemCamera);
    ++m_cameraId;
}

void MainWindow::slotSwitchCamera()
{
    vector<string> selectedStrVec = getSelectedVector();
    if (selectedStrVec.size() == 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("请选择一个摄像机条目"));
        return;
    }

    //找到摄像机，然后切换到该摄像机
    emit sigSwitchCamera(selectedStrVec);
}

void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // 获取新增的选中项
    QModelIndexList newlySelectedIndexes = selected.indexes();
    if (!newlySelectedIndexes.isEmpty())
    {
        QModelIndex currentIndex = ui->treeView_instance->currentIndex();
        if (currentIndex.isValid())
        {
            string currentItemText = m_model->data(currentIndex, Qt::DisplayRole).toString().toStdString();
            cout << "last item: "<<currentItemText<<endl;
            //这里判断属于哪个parent条目下，直接调用GLWidget获取资源指针
            QStandardItem* parentItem = m_model->itemFromIndex(currentIndex.parent());
            if (parentItem == m_itemObject)
            {
                Object* ptr = m_glwidget->queryLastItem<Object*>(currentItemText);
                if (ptr != nullptr)
                //处理物体类显示信息
                    showObjectInfo(ptr);
            }
            else if (parentItem == m_itemLight)
            {
                Light* ptr = m_glwidget->queryLastItem<Light*>(currentItemText);
                //处理灯光类显示信息
                if (ptr != nullptr)
                    showLightInfo(ptr);
            }
            else if (parentItem == m_itemCamera)
            {
                Camera* ptr = m_glwidget->queryLastItem<Camera*>(currentItemText);
                if (ptr != nullptr)
                    showCameraInfo(ptr);
            }
        }
        else
        {
            qDebug() << "No focus item.";
        }
    }
}

void MainWindow::slotRenderImage()
{
    cv::Mat frame = m_glwidget->captureFrame();
    m_renderImageWidget = new renderImageWidget(frame);
    m_renderImageWidget->show();
}

void MainWindow::slotOpenUVTextureDialog()
{
    //在这里添加判断，是否选中了物体的某一个
    if (ui->comboBox_textureType->currentIndex() <=2)
    {
        QModelIndexList selectedIndexes = ui->treeView_instance->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty())
        {
            QMessageBox::warning(this, tr("Warning"), tr("请选择一个物体"));
            return ;
        }
        QStandardItem* parentItem = m_model->itemFromIndex(selectedIndexes[0].parent());
        if (parentItem != m_itemObject)
        {
            QMessageBox::warning(this, tr("Warning"), tr("请选择物体而不是光源或者摄像机"));
            return ;
        }
    }
    m_fileDialog->setGeometry(1000, 100, 650, 400);
    m_fileDialog->show();
}

void MainWindow::slotpassUVTextureInfo(const QString &s)
{   
    wstring filePath = s.toStdWString();
    int index = ui->comboBox_textureType->currentIndex();
    if (index == 0 || index == 1 || index == 2)
    {
        QModelIndex currentIndex = ui->treeView_instance->currentIndex();
        string currentItemText = m_model->data(currentIndex, Qt::DisplayRole).toString().toStdString();
        TextureType textureType  = index==0?TextureType::DIFFUSE_MAP:(index == 1?TextureType::NORMAL_MAP:TextureType::CUBE_MAP);
        emit sigSetObjectUVTexture(currentItemText, filePath, textureType);
    }
    else
    {
        //HDR贴图，它不需要选择物体
        emit sigCreateHDRMap(filePath);
    }



}

void MainWindow::slotRemoveUVTexture()
{
    QModelIndex currentIndex = ui->treeView_instance->currentIndex();
    string currentItemText = m_model->data(currentIndex, Qt::DisplayRole).toString().toStdString();
   // QStandardItem* item = m_model->itemFromIndex(currentIndex);
    int index = ui->comboBox_textureType->currentIndex();
    TextureType textureType  = index==0?TextureType::DIFFUSE_MAP:(index == 1?TextureType::NORMAL_MAP:TextureType::CUBE_MAP);
    emit sigRemoveUVTextures(currentItemText, textureType);
}

void MainWindow::resetObjectId()
{
    m_objectId = 1;
}
void MainWindow::resetLightId()
{
    m_lightId = 1;
}
void MainWindow::resetCameraId()
{
    m_cameraId = 1;
}

void MainWindow::showObjectInfo(Object *ptr)
{
    //获取物体信息显示到属性栏中
    //主要是物体属性和材质属性
    glm::vec3 position = ptr->getPosition();
    ui->lineEdit_translationX->setText(QString::number(position.x));
    ui->lineEdit_translationY->setText(QString::number(position.y));
    ui->lineEdit_translationZ->setText(QString::number(position.z));
    glm::vec3 rotation = ptr->getRotation();
    ui->lineEdit_rotationX->setText(QString::number(rotation.x));
    ui->lineEdit_rotationY->setText(QString::number(rotation.y));
    ui->lineEdit_rotationZ->setText(QString::number(rotation.z));
    glm::vec3 scale = ptr->getScale();
    ui->lineEdit_scaleX->setText(QString::number(scale.x));
    ui->lineEdit_scaleY->setText(QString::number(scale.y));
    ui->lineEdit_scaleZ->setText(QString::number(scale.z));

    vector<float> texture = ptr->getMaterialValue();
    ui->lineEdit_metallic->setText(QString::number(texture[0]));
    ui->lineEdit_roughness->setText(QString::number(texture[1]));
    ui->lineEdit_ior->setText(QString::number(texture[2]));
}

void MainWindow::showLightInfo(Light *ptr)
{
    //获取灯光信息显示到属性栏中
    Type lightType = ptr->getLightType();
    ShaderLight sl;
    if ( lightType == Type::Point)
    {
        sl = convertToShaderLight(*static_cast<PointLight*>(ptr));

    }
    else if (lightType == Type::Directional)
    {
        sl = convertToShaderLight(*static_cast<DirectionalLight*>(ptr));

    }
    else if (lightType == Type::Spot)
    {
        sl = convertToShaderLight(*static_cast<SpotLight*>(ptr));

    }
    else
    {
        cout<<"unknown light type"<<endl;
    }
    ui->lineEdit_lightLocateX->setText(QString::number(sl.position.x));
    ui->lineEdit_lightLocateY->setText(QString::number(sl.position.y));
    ui->lineEdit_lightLocateZ->setText(QString::number(sl.position.z));

    ui->lineEdit_lightIntensity->setText(QString::number(sl.intensity));

    ui->lineEdit_lightDirectorX->setText(QString::number(sl.direction.x));
    ui->lineEdit_lightDirectorY->setText(QString::number(sl.direction.y));
    ui->lineEdit_lightDirectorZ->setText(QString::number(sl.direction.z));

    ui->lineEdit_lightInnerConeAngle->setText(QString::number(sl.innerAngle));
    ui->lineEdit_lightOuterConeAngle->setText(QString::number(sl.outerAngle));
    //点光源和聚光灯的衰减参数等，
    ui->lineEdit_quadratic->setText(QString::number(sl.quadratic));
    ui->lineEdit_liner->setText(QString::number(sl.linear));
    ui->lineEdit_constant->setText(QString::number(sl.constant));
}

void MainWindow::showCameraInfo(Camera *ptr)
{
    //获取摄像机信息显示到属性栏中
    ui->combo_cameraType->setCurrentIndex(ptr->getIsUseOrthographic());
    ui->lineEdit_cameraFOV->setText(QString::number(ptr->getFOV()));
    ui->lineEdit_cameraYaw->setText(QString::number(ptr->getYaw()));
    ui->lineEdit_cameraPitch->setText(QString::number(ptr->getPitch()));
    ui->lineEdit_clipNear->setText(QString::number(ptr->getNearClip()));
    ui->lineEdit_clipFar->setText(QString::number(ptr->getFarClip()));

    ui->lineEdit_translationX->setText(QString::number(ptr->getCameraPosition().x));
    ui->lineEdit_translationY->setText(QString::number(ptr->getCameraPosition().y));
    ui->lineEdit_translationZ->setText(QString::number(ptr->getCameraPosition().z));
}

void MainWindow::slotChangeItemName(const QModelIndex &index, const QString &oldValue, const QString &newValue)
{
    string sold = oldValue.toStdString();
    string snew = newValue.toStdString();
    m_glwidget->renameOneItem(sold, snew);
}
void MainWindow::removeItems()
{
    QModelIndexList selectedIndexes = ui->treeView_instance->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty())
    {
        // 为了避免在遍历和修改模型的同时引起的问题，我们可以先收集要删除的项
        unordered_set<QStandardItem*> itemsToRemove;
        vector<string> strRemove;

        for (const QModelIndex &index : selectedIndexes)
        {
            QStandardItem* item = m_model->itemFromIndex(index);
            if (item && !itemsToRemove.count(item))
            {
                // 获取条目的名称
                string itemName = item->text().toStdString();
                strRemove.emplace_back(itemName);
                // 将需要删除的条目添加到列表中
                itemsToRemove.insert(item);
            }
        }
        emit sigRemove(strRemove);
        // 删除收集到的所有条目
        for (QStandardItem* item : itemsToRemove)
        {
            if (item->parent())
            {
                item->parent()->removeRow(item->row());
            }
            else
            {
               // m_model->removeRow(item->row());
                QMessageBox::warning(this, tr("Warning"), tr("不能删除该条目"));
            }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("请在实例管理中至少选择一条去删除"));
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    ui->openGLWidget_main->setGeometry(10, 10, width() * 3.0f/4.0f, width() * 3.0f/4.0f * 9.0f/16.0f);
    ui->treeView_instance->setGeometry(3.9f/5.0f *width() , 10, width() * 1.0f/5.0f, width() * 1.1f/5.0f);
    ui->gridLayoutWidget->setGeometry(10,width() * 3.0f/4.0f * 9.0f/16.0f -40, 2.0f/5.0f * width(), 1.0f/5.0f * width());
    ui->tabWidget_attribute->setGeometry(3.9f/5.0f *width(), width() * 1.1f/5.0f + 20, width() * 1.0f/5.0f+10, width() * 1.2f/5.0f);
    if (m_glwidget != nullptr)
        m_glwidget->resize(ui->openGLWidget_main->width(), ui->openGLWidget_main->height());
    m_colorDialog->setGeometry(20, 50, 100, 100);
        qDebug() << "glWidth:" << m_glwidget->width() << "Height:" << m_glwidget->height();
}


void MainWindow::keyReleaseEvent(QKeyEvent* ev)
{
    if (ev->key() == Qt::Key_Delete)
    {
        removeItems();
    }
    else if (ev->key() == Qt::Key_Return)
    {
        //进行矩阵变换，材质添加等，需要先在实例管理中选定某个条目才更新
        vector<string> selectedStrVec = getSelectedVector();
        if (selectedStrVec.size() == 0)
        {
            QMessageBox::warning(this, tr("Warning"), tr("请在实例管理中至少选择一各个物体去更改"));
            return ;
        }
        else if (selectedStrVec.size() > 1)
        {
            QMessageBox::warning(this, tr("Warning"), tr("暂时只能选择一个物体去更改"));
            return ;
        }
        //模型矩阵变换
        //先做下判断，如果输入不是数字，则弹框重新输入

        //根据输入设置模型矩阵，传递给glwidget设置各个个物体的模型矩阵。
        float translationX = ui->lineEdit_translationX->text().toFloat();
        float translationY = ui->lineEdit_translationY->text().toFloat();
        float translationZ = ui->lineEdit_translationZ->text().toFloat();
        glm::vec3 translationXYZ(translationX, translationY, translationZ);

        float rotationX = ui->lineEdit_rotationX->text().toFloat();
        float rotationY = ui->lineEdit_rotationY->text().toFloat();
        float rotationZ = ui->lineEdit_rotationZ->text().toFloat();
        glm::vec3 rotationXYZ(rotationX, rotationY, rotationZ);

        float scaleX = ui->lineEdit_scaleX->text().toFloat();
        float scaleY = ui->lineEdit_scaleY->text().toFloat();
        float scaleZ = ui->lineEdit_scaleZ->text().toFloat();
        glm::vec3 scaleXYZ(scaleX, scaleY, scaleZ);

        emit sigSetModel(selectedStrVec, translationXYZ, rotationXYZ, scaleXYZ);

        float metallic = ui->lineEdit_metallic->text().toFloat();
        float roughness = ui->lineEdit_roughness->text().toFloat();
        float ior = ui->lineEdit_ior->text().toFloat();
        emit sigSetMaterialValue(selectedStrVec, metallic, roughness, ior);

        //接下来是灯光的属性设置，设置一个shaderLight结构体传递给GLWidget
        ShaderLight sl;
        sl.intensity = ui->lineEdit_lightIntensity->text().toFloat();
        sl.position = glm::vec4(ui->lineEdit_lightLocateX->text().toFloat(),ui->lineEdit_lightLocateY->text().toFloat(),ui->lineEdit_lightLocateZ->text().toFloat(), 1.0f);
        sl.quadratic = ui->lineEdit_quadratic->text().toFloat();
        sl.linear = ui->lineEdit_liner->text().toFloat();
        sl.constant = ui->lineEdit_constant->text().toFloat();
        sl.innerAngle = ui->lineEdit_lightInnerConeAngle->text().toFloat();
        sl.outerAngle = ui->lineEdit_lightOuterConeAngle->text().toFloat();
        sl.direction = glm::vec4(ui->lineEdit_lightDirectorX->text().toFloat(), ui->lineEdit_lightDirectorY->text().toFloat(),ui->lineEdit_lightDirectorZ->text().toFloat(), 1.0f);
        emit sigSetLightAttribute(selectedStrVec, sl);
        //摄像机的属性设置
        int cameratype = ui->combo_cameraType->currentIndex();
        float fov = ui->lineEdit_cameraFOV->text().toFloat();
        float yaw = ui->lineEdit_cameraYaw->text().toFloat();
        float pitch = ui->lineEdit_cameraPitch->text().toFloat();
        float clipNear = ui->lineEdit_clipNear->text().toFloat();
        float clipFar = ui->lineEdit_clipFar->text().toFloat();
        glm::vec3 position;
        position.x = ui->lineEdit_translationX->text().toFloat();
        position.y = ui->lineEdit_translationY->text().toFloat();
        position.z = ui->lineEdit_translationZ->text().toFloat();
        //摄像机的旋转和缩放暂时不加
        emit sigSetCameraAttribute(selectedStrVec, cameratype, fov, yaw, pitch, clipNear, clipFar, position);

    }
}
